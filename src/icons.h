#pragma once
#include <string>
#include <unordered_map>

struct Icons {
	static std::unordered_map<std::string, std::string> distroIconMap;
	static std::string iconUser, iconHname, iconDistro, iconKernel, iconUptime, iconShell, iconDeWm, iconPkgs, iconMemory, iconColors, iconColorSwatches;
	static bool showNothing, showUsername, showHostname, showDistro, showKernel, showUptime, showShell, showDeWm, showPkg, showMemory, showColors;
};
