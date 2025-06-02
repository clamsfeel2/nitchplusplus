#include "logos.h"
#include "system_info.h"
#include "icons.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <array>
#if defined(__APPLE__) && defined(__MACH__)
    #include <mach/mach.h>
    #include <sys/sysctl.h>
	#include <unistd.h>
    #include <cstring>
#else
    #include <algorithm>
    #include <fstream>
#endif

std::string SystemInfo::s_distroID;
std::string SystemInfo::s_logo;

void SystemInfo::Initialize(bool getLogos) {
	if(getLogos) s_logo = Logos::GetLogos(SystemInfo::s_distroID);
	Icons icon;
	distro        = (icon.s_showDistro ? GetDistro() : "");          // Uses /etc/os-release
	hostname      = (icon.s_showHostname ? GetHostname() : "");      // /proc/sys/kernel/hostname
	kernel        = (icon.s_showKernel ? GetKernel() : "");          // /proc/sys/kernel/osrelease
	shell         = (icon.s_showShell ? GetShell() : "");            // SHELL env var
	user          = (icon.s_showUsername ? GetUser() : "");          // USER env var
	packageCount  = (icon.s_showPkgs ? GetPackagesByDistro() : "0");  // Each distros package dir in /var/*
	uptime        = (icon.s_showUptime ? GetUptime() : "");          // /proc/uptime
	memory        = (icon.s_showMemory ? GetMemoryUsage() : "");     // /proc/meminfo
	de            = (icon.s_showDeWm ? GetDesktopEnv() : "");        // XDG_CURRENT_DESKTOP, CURRENT_DESKTOP, DE env vars, aqua for mac (prob not the best to hardcode it... FIXME?)
}

std::string SystemInfo::Exec(const char* command) {
	std::array<char, 128> buffer;
	std::string result;
	auto closePipe = [](FILE* file) { pclose(file); };
	std::unique_ptr<FILE, decltype(closePipe)> pipe(popen(command, "r"), closePipe);

	if(!pipe) throw std::runtime_error("Exec method failed!");
	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();

	return result;
}

std::string SystemInfo::GetMemoryUsage() {
    double totalMemoryGiB = 0.0, availableMemoryGiB = 0.0;

    #if defined(__APPLE__) && defined(__MACH__)
        int64_t totalMemoryBytes = 0;
        size_t size = sizeof(totalMemoryBytes);
        if(sysctlbyname("hw.memsize", &totalMemoryBytes, &size, NULL, 0) != 0) {
            return "NULL";
        }

        mach_port_t host_port = mach_host_self();
        vm_size_t page_size;
        mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
        vm_statistics64_data_t vstats;

        if(host_page_size(host_port, &page_size) != KERN_SUCCESS || host_statistics64(host_port, HOST_VM_INFO, (host_info64_t)&vstats, &count) != KERN_SUCCESS) {
            return "NULL";
        }

        int64_t availableMemoryBytes = (vstats.free_count + vstats.inactive_count) * page_size;
        totalMemoryGiB = static_cast<double>(totalMemoryBytes) / (1024.0 * 1024.0 * 1024.0);
        availableMemoryGiB = static_cast<double>(availableMemoryBytes) / (1024.0 * 1024.0 * 1024.0);

    #else
        std::ifstream meminfoFile("/proc/meminfo");
        if(!meminfoFile.is_open()) {
            return "NULL";
        }

		std::string key, value;
		while(getline(meminfoFile, key)) {
			std::istringstream lineStream(key);
			if(lineStream >> key >> value) {
				if(key == "MemTotal:") totalMemoryGiB = std::stod(value) / (1024.0 * 1024.0);
				else if(key == "MemAvailable:") availableMemoryGiB = std::stod(value) / (1024.0 * 1024.0);
			}
		}
        meminfoFile.close();

        if(totalMemoryGiB == 0.0 || availableMemoryGiB == 0.0) {
            return "NULL";
        }
    #endif

    double usedMemoryGiB = totalMemoryGiB - availableMemoryGiB;
    std::ostringstream outputStr;
    outputStr.precision(2);
    outputStr << std::fixed << usedMemoryGiB << " GiB | " << totalMemoryGiB << " GiB";

    return outputStr.str();
}

std::string SystemInfo::GetUptime() {
    double uptimeSeconds = 0.0;

    #if defined(__APPLE__) && defined(__MACH__)
        struct timeval boottime;
        size_t size = sizeof(boottime);
        if(sysctlbyname("kern.boottime", &boottime, &size, NULL, 0) != 0 || boottime.tv_sec == 0) {
            return "NULL";
        }
        uptimeSeconds = difftime(time(NULL), boottime.tv_sec);

    #else
        std::ifstream uptimeFile("/proc/uptime");
        if(!uptimeFile.is_open()) {
            return "NULL";
        }
        std::string line;
        getline(uptimeFile, line);
        std::istringstream iss(line);
        iss >> uptimeSeconds;
        uptimeFile.close();
    #endif

    // Convert seconds into days, hours, and minutes
    int days = static_cast<int>(uptimeSeconds / (24 * 3600));
    uptimeSeconds -= days * 24 * 3600;
    int hours = static_cast<int>(uptimeSeconds / 3600);
    uptimeSeconds -= hours * 3600;
    int minutes = static_cast<int>(uptimeSeconds / 60);

    std::ostringstream formattedUptime;
    if(days > 0) formattedUptime << days << "d ";
    if(hours > 0) formattedUptime << hours << "h ";
    formattedUptime << minutes << "m";

    return formattedUptime.str();
}

std::string SystemInfo::GetDistro() {
    #if defined(__APPLE__) && defined(__MACH__)
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("sw_vers -productName && sw_vers -productVersion", "r"), pclose);
        if(!pipe) {
            return "NULL";
        }
        while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        // Remove trailing newlines
        distro = result;
        distro.erase(std::remove(distro.begin(), distro.end(), '\n'), distro.end());
    #else
        std::ifstream inputFile("/etc/os-release");
        if(!inputFile.is_open()) {
            return "NULL";
        }
        std::string line, key, value;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            if(std::getline(iss, key, '=') && std::getline(iss, value)) {
                if(!value.empty() && value.front() == '\"') {
                    value.erase(0, 1);
                }
                if(!value.empty() && value.back() == '\"') {
                    value.pop_back();
                }
                if(key == "PRETTY_NAME") {
                    distro = value;
                    break;
                }
            }
        }
        inputFile.close();
    #endif

    return distro;
}

void SystemInfo::InitializeDistroID() {
	#if defined(__APPLE__) && defined(__MACH__)
        SystemInfo::s_distroID = "macos";
    #else
	// Retreiving the ID value form /etc/os-release
    std::ifstream inputFile("/etc/os-release");
    if(!inputFile.is_open()) {
        return;
    }
    std::string line;
    while(std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if(std::getline(iss, key, '=') && std::getline(iss, value)) {
            if(key == "ID") {
                if(!value.empty() && value.front() == '\"') value.erase(0, 1);
                if(!value.empty() && value.back() == '\"') value.pop_back();
                SystemInfo::s_distroID = value;
                break;
            }
        }
    }
    inputFile.close();
	#endif
}

std::string SystemInfo::GetHostname() {
#if defined(__APPLE__) && defined(__MACH__)
	char buffer[256];
	if(gethostname(buffer, sizeof(buffer)) == 0) {
		std::string host(buffer);
		if(host.size() > 6 && host.substr(host.size() - 6) == ".local") {
			return host.substr(0, host.size() - 6);
		}
		return host;
	}
	return "NULL";
#elif __linux__
	std::ifstream inputFile("/proc/sys/kernel/hostname");
	if(!inputFile.is_open()) {
		return "NULL";
	}
	std::string line;
	while (std::getline(inputFile, line)) {
		return line;
	}
#endif
	return "NULL";
}

std::string SystemInfo::GetKernel() {
	std::string line, ker;
	std::ifstream inputFile("/proc/sys/kernel/osrelease");
	if(!inputFile.is_open()) {
		ker = Exec("uname -r");
		if(!ker.empty()) {
			return ker;
		} else {
			return "NULL";
		}
	}
	while(std::getline(inputFile, line)) {
		return line;
	}
	return "NULL";
}

std::string SystemInfo::GetDesktopEnv() {
	#if defined(__APPLE__) && defined(__MACH__)
    static const std::vector<std::string> wms = { "yabai", "amethyst", "loop" };
    for(const std::string& wm : wms) {
        std::string cmd = "pgrep -q " + wm + " 2>/dev/null";
        if(system(cmd.c_str()) == 0) {
            return wm;
        }
    }
    return "Aqua";
	#else
		const char* d = std::getenv("XDG_CURRENT_DESKTOP") ?: std::getenv("DESKTOP_SESSION") ?: std::getenv("DE");
		return d ? std::string(d) : "";
	#endif
}

std::string SystemInfo::GetShell() {
	if(const char* shellEnv = std::getenv("SHELL")) {
		shell = shellEnv;
		size_t lastSlashIndex = shell.find_last_of('/');
		return lastSlashIndex == std::string::npos ? shell : shell.erase(0, lastSlashIndex + 1);
	}
	return "NULL";
}

std::string SystemInfo::GetUser() {
    if(char* u = std::getenv("USER"); u && *u) return u;
    return Exec("whoami");
}

std::string SystemInfo::GetPackagesByDistro() {
    const std::unordered_map<std::string, std::string> distroPkgPathMap = {
        // Pacman family
        { "arch",      "/var/lib/pacman/local" },
        { "manjaro",   "/var/lib/pacman/local" },
        { "artix",     "/var/lib/pacman/local" },
        { "endeavouros","/var/lib/pacman/local" },
        // Dpkg family
        { "debian",    "/var/lib/dpkg/info" },
        { "ubuntu",    "/var/lib/dpkg/info" },
        { "mint",      "/var/lib/dpkg/info" },
        { "zorin",     "/var/lib/dpkg/info" },
        { "popos",     "/var/lib/dpkg/info" },
        // Rpm family
        { "redhat",    "/var/lib/rpm" },
        { "fedora",    "/var/lib/rpm" },
        { "centos",    "/var/lib/rpm" },
        { "slackware", "/var/lib/rpm" },
        // Others
        { "opensuse",  "/var/lib/zypp/db" },
        { "gentoo",    "/var/db/pkg" },
        { "flatpak",   "/var/lib/flatpak/app" },
        { "void",      "/var/db/xbps" }
    };

    auto countSubDirs = [&](const std::string& path) -> int {
        int cnt = 0;
        std::error_code err;
        for(auto& dirEntry : std::filesystem::directory_iterator(path, std::filesystem::directory_options::skip_permission_denied, err)) {
            if(dirEntry.is_directory()) ++cnt;
        }
        return cnt;
    };

    // For pretty much all of the distros
    if(auto it = distroPkgPathMap.find(s_distroID); it != distroPkgPathMap.end()) {
        return std::to_string(countSubDirs(it->second));
    }

    // NixOS (because I haven't learned how nix pkg system works yet)
    if(s_distroID == "nixos") return Exec("nix-env --query --installed | wc -l");

    // MacOS
    if(s_distroID == "macos") {
        std::ostringstream out;

        int sysCount = std::stoi(Exec("pkgutil --pkgs | wc -l"));
        out << sysCount << " (system)";

        // Homebrew
        const char* brewEnv = std::getenv("HOMEBREW_PREFIX");
        std::string brewPrefix = brewEnv ? brewEnv : "/opt/homebrew";
        int brewFormulae = countSubDirs(brewPrefix + "/Cellar");
        if(brewFormulae > 0) out << ", " << brewFormulae << " (brew)";
        int brewCask = countSubDirs(brewPrefix + "/Caskroom");
        if(brewCask > 0) out << ", " << brewCask << " (brew cask)";

        // Macports
        const char* portsEnv = std::getenv("MACPORTS_PREFIX");
        std::string portsPrefix = portsEnv ? portsEnv : "/opt/local";
        int macportsCount = countSubDirs(portsPrefix + "/var/macports/software");
        if(macportsCount > 0) out << ", " << macportsCount << " (macports)";

        // Nixenv (see above)
        int nixCount = std::stoi(Exec("nix-env --query --installed 2>/dev/null | wc -l"));
        if(nixCount > 0) out << ", " << nixCount << " (nix)";

        return out.str();
    }
    return "-1";
}

