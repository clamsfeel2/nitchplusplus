#pragma once
#include <string>
#include <unordered_map>

struct Icons {
	static std::unordered_map<std::string, std::string> distroIconMap;
	static std::string iconUser;
	static std::string iconHname;
	static std::string iconDistro;
	static std::string iconKernel;
	static std::string iconUptime;
	static std::string iconShell;
	static std::string iconDeWm;
	static std::string iconPkgs;
	static std::string iconMemory;
	static std::string iconColors;
	
	static bool showNothing;
	static bool showUsername;
	static bool showHostname;
	static bool showDistro;
	static bool showKernel;
	static bool showUptime;
	static bool showShell;
	static bool showDeWm;
	static bool showPkg;
	static bool showMemory;
	static bool showColors;
};
