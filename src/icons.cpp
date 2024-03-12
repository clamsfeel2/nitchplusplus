#include "icons.h"

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
std::string	Icons::iconUser("");
std::string Icons::iconHname("");
std::string Icons::iconKernel("󰌢");
std::string Icons::iconUptime("");
std::string Icons::iconDistro("󰻀");
std::string Icons::iconShell("");
std::string Icons::iconDeWm("");
std::string Icons::iconPkgs("󰏖");
std::string Icons::iconMemory("󰍛");
std::string Icons::iconColors("");
std::string Icons::iconColorSwatches("");

bool Icons::showNothing(false);
bool Icons::showUsername(true);
bool Icons::showHostname(true);
bool Icons::showDistro(true);
bool Icons::showKernel(true);
bool Icons::showUptime(true);
bool Icons::showShell(true);
bool Icons::showDeWm(true);
bool Icons::showPkg(true);
bool Icons::showMemory(true);
bool Icons::showColors(true);
