#include "logos.h"
#include "system_info.h"
#include "icons.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <array>

std::string SystemInfo::distroID;
std::string SystemInfo::logo;

void SystemInfo::Initialize(bool getLogos) {
	if(getLogos) { 
		logo = Logos::GetLogos(SystemInfo::distroID); // logos.h
	}
	Icons icon;
	distro = (icon.showDistro ? GetDistro() : ""); // Uses /etc/os-release
	hostname = (icon.showHostname ? GetHostname() : ""); // /proc/sys/kernel/hostname
	kernel = (icon.showKernel ? GetKernel() : ""); // /proc/sys/kernel/osrelease
	shell = (icon.showShell ? GetShell() : ""); // SHELL env var
	user = (icon.showUsername ? GetUser() : ""); // USER env var
	packageCount = (icon.showPkg ? GetPackagesByDistro() : 0); // Each distros package dir in /var/*
	uptime = (icon.showUptime ? GetUptime() : ""); // /proc/uptime
	memory = (icon.showMemory ? GetMemoryUsage() : ""); // /proc/meminfo
	de = (icon.showDeWm ? GetDesktopEnv() : ""); // XDG_CURRENT_DESKTOP, CURRENT_DESKTOP, DE env vars
}

std::string SystemInfo::Exec(const char* command) {
	std::array<char, 128> buffer;
	std::string result;
	auto closePipe = [](FILE* file) { pclose(file); };
	std::unique_ptr<FILE, decltype(closePipe)> pipe(popen(command, "r"), closePipe);

	if(!pipe) {
		throw std::runtime_error("Exec method failed!");
	}
	while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
} // ends Exec()

std::string SystemInfo::GetMemoryUsage() {
	std::string line, totalMemory, availableMemory;
	std::ifstream meminfoFile("/proc/meminfo");

	if(!meminfoFile.is_open()) {
		return "";
	}
	while(getline(meminfoFile, line)) {
		std::istringstream iss(line);
		std::string key, value;
		iss >> key >> value;
		if(key == "MemTotal:") {
			totalMemory = value;
		} else if(key == "MemAvailable:") {
			availableMemory = value;
		}
	}
	meminfoFile.close();
	if(totalMemory.empty() || availableMemory.empty()) {
		std::cerr << "Error parsing /proc/meminfo" << std::endl;
		return "";
	}

	double totalMemoryGiB = std::stod(totalMemory) / (1024.0 * 1024.0);
	double availableMemoryGiB = std::stod(availableMemory) / (1024.0 * 1024.0);
	double result = totalMemoryGiB - availableMemoryGiB;
	std::ostringstream outputStr;
	outputStr.precision(2);
	outputStr << std::fixed << result << " GiB | " << totalMemoryGiB << " GiB";

	return outputStr.str();
} // ends GetMemoryInfo()

std::string SystemInfo::GetUptime() {
	std::ifstream uptimeFile("/proc/uptime");

	if(!uptimeFile.is_open()) {
		return "";
	}
	// Reading information from file into vars
	std::string line;
	getline(uptimeFile, line);
	std::istringstream iss(line);
	double uptimeSeconds, idleSeconds;
	iss >> uptimeSeconds >> idleSeconds;
	uptimeFile.close();
	// Converting the seconds into days, hours, and minutes
	int days = static_cast<int>(uptimeSeconds / (24 * 3600));
	uptimeSeconds -= days * 24 * 3600;
	int hours = static_cast<int>(uptimeSeconds / 3600);
	uptimeSeconds -= hours * 3600;
	int minutes = static_cast<int>(uptimeSeconds / 60);

	std::ostringstream formattedUptime;
	formattedUptime << days << "d " << hours << "h " << minutes << "m";

	return formattedUptime.str();
} // ends GetUptime()

std::string SystemInfo::GetDistro() {
    std::string key, value;
    std::ifstream inputFile("/etc/os-release");
    if(!inputFile.is_open()) {
        return "";
    }
    std::string line;
    while(std::getline(inputFile, line)) {
        std::istringstream iss(line);

        if(std::getline(iss, key, '=') && std::getline(iss, value)) {
            if(!value.empty() && value.front() == '\"') {
                value.erase(0, 1);
            }
            if(!value.empty() && value.back() == '\"') {
                value.pop_back();
            }

            if(key == "PRETTY_NAME") {
                return value;
            }
        }
    }
    inputFile.close();
    return "";
}

// // Retreiving the ID value form /etc/os-release
void SystemInfo::InitializeDistroID() {
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
                if(!value.empty() && value.front() == '\"') {
                    value.erase(0, 1);
                }
                if(!value.empty() && value.back() == '\"') {
                    value.pop_back();
                }
                SystemInfo::distroID = value;
                break;
            }
        }
    }
    inputFile.close();
}

std::string SystemInfo::GetHostname() {
	std::ifstream inputFile("/proc/sys/kernel/hostname");
	if(!inputFile.is_open()) {
		return ""; 
	}
	std::string line;
	while(std::getline(inputFile, line)) {
		std::istringstream iss(line);
		std::string value;
		if(std::getline(iss, value)) {
			return value;
		}
	}
	return "";
} // ends GetHostname()

std::string SystemInfo::GetKernel() {
	std::string line, ker;
	std::ifstream inputFile("/proc/sys/kernel/osrelease");
	if(!inputFile.is_open()) {
		ker = Exec("uname -r");
		if(!ker.empty()) {
			return ker;
		} else {
			return "";
		}
	}
	while(std::getline(inputFile, line)) {
		return line;
	}
	return ""; 
} // ends GetKernel()

std::string SystemInfo::GetDesktopEnv() {
	const char* d;
	if((d = std::getenv("XDG_CURRENT_DESKTOP")) || (d = std::getenv("DESKTOP_SESSION")) || (d = std::getenv("DE"))) {
		return std::string(d);
	}
	return "";
} // ends GetDesktopEnv()

std::string SystemInfo::GetShell() {
	if(const char* shellEnv = std::getenv("SHELL")) {
		shell = shellEnv;
		size_t lastSlashIndex = shell.find_last_of('/');
		return (lastSlashIndex == std::string::npos ? shell : shell.erase(0, lastSlashIndex + 1));
	}
	return "";
}

std::string SystemInfo::GetUser() {
	const char* u = std::getenv("USER");
	if(u != nullptr && *u != '\0') {
		return std::string(u);
	} else {
		return "";
	}
} // ends GetUser()

// Finds packages based on amount of items in /var/lib/* dir or /var/db for gentoo
int SystemInfo::GetPackagesByDistro() {
	std::string packageDir;
	// Find way to use directory to get nix pkgs
	std::string nixCommand = "nix-env --query --installed | wc -l";

	if(SystemInfo::distroID == "arch" || SystemInfo::distroID == "manjaro" || SystemInfo::distroID == "artix" || SystemInfo::distroID == "endeavouros") {
		packageDir = "/var/lib/pacman/local";
	} else if(SystemInfo::distroID == "debian" || SystemInfo::distroID == "ubuntu" || SystemInfo::distroID == "mint" || SystemInfo::distroID == "zorin" || SystemInfo::distroID == "popos") {
		packageDir = "/var/lib/dpkg/info";
	} else if(SystemInfo::distroID == "redhat" || SystemInfo::distroID == "fedora" || SystemInfo::distroID == "centos" || SystemInfo::distroID == "slackware") {
		packageDir = "/var/lib/rpm";
	} else if(SystemInfo::distroID == "opensuse") {
		packageDir = "/var/lib/zypp/db";
	} else if(SystemInfo::distroID == "gentoo") {
		packageDir = "/var/db/pkg";
	} else if(SystemInfo::distroID == "flatpak") {
		packageDir = "/var/lib/flatpak/app";
	} else if(SystemInfo::distroID == "void") {
		packageDir = "/var/db/xbps";
	} else if(SystemInfo::distroID == "nixos") {
		return std::stoi(Exec(nixCommand.c_str()));
	} else { 
		std::cerr << "Unsupported distribution: " << SystemInfo::distroID << std::endl;
		return -1;
	}
	try {
		unsigned int directoryCount = 0;
		// Iterate over directory and count all sub-directories (pkgs)
		for(const auto& entry : std::filesystem::directory_iterator(packageDir)) {
			if(std::filesystem::is_directory(entry.path())) {
				directoryCount++;
			}
		}
		return directoryCount;
	} catch(const std::filesystem::filesystem_error& e) {
		std::cerr << "Error accessing package directory: " << e.what() << std::endl;
		return -1;
	}
} // ends GetPackagesByDistro()
