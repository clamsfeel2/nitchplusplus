#pragma once
#include <string>

struct SystemInfo {
	static std::string s_distroID, s_logo;
	std::string distro, hostname, kernel, shell, user, uptime, memory, de, packageCount;

	std::string Exec(const char* command);
	void Initialize(bool getLogos);
	void InitializeDistroID();
	std::string GetUser();
	std::string GetHostname();
	std::string GetDistro();
	std::string GetKernel();
	std::string GetUptime();
	std::string GetShell();
	std::string GetDesktopEnv();
	std::string GetPackagesByDistro();
	std::string GetMemoryUsage();
};
