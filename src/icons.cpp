#include "icons.h"
#include "system-info.h"

	std::unordered_map<std::string, std::string> Icons::distroIconMap = {
		{"ubuntu", ""},
		{"arch", "󰣇"},
		{"debian", ""},
		{"fedora", ""},
		{"mint", "󰣭"},
		{"zorin", ""},
		{"popos", ""},
		{"manjaro", ""},
		{"opensuse", ""},
		{"centos", ""},
		{"gentoo", ""},
		{"endeavouros", ""},
		{"artix", ""}, 
		{"void", ""},
		{"nixos", ""}
	};
std::string	Icons::iconUser = "";
std::string Icons::iconHname = "";
std::string Icons::iconKernel = "󰌢";
std::string Icons::iconUptime = "";
// Lambda uses distroID to find matching icon in distroIconMap and returns penguin if distro not found
std::string Icons::iconDistro = []() -> std::string { auto it = distroIconMap.find(SystemInfo::distroID); return (it != distroIconMap.end()) ? it->second : "󰻀"; }();
std::string Icons::iconShell = "";
std::string Icons::iconDeWm = "";
std::string Icons::iconPkgs = "󰏖";
std::string Icons::iconMemory = "󰍛";
std::string Icons::iconColors = "";

bool Icons::showNothing = false;
bool Icons::showUsername;
bool Icons::showHostname;
bool Icons::showDistro;
bool Icons::showKernel;
bool Icons::showUptime;
bool Icons::showShell;
bool Icons::showDeWm;
bool Icons::showPkg;
bool Icons::showMemory;
bool Icons::showColors;
