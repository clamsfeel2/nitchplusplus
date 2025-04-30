#pragma once
#include <string>
#include <unordered_map>

struct Icons {
	static std::unordered_map<std::string, std::string> s_distroIconMap;
	static std::string s_iconUser, s_iconHname, s_iconDistro, s_iconKernel, s_iconUptime, s_iconShell, s_iconDeWm, s_iconPkgs, s_iconMemory, s_iconColors, s_iconColorSwatches;
	static bool s_showNothing, s_showUsername, s_showHostname, s_showDistro, s_showKernel, s_showUptime, s_showShell, s_showDeWm, s_showPkgs, s_showMemory, s_showColors;
};
