#include "printer.h"
#include "configuration.h"
#include "system_info.h"
#include "icons.h"
#include "colors.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

// FIXME: Organize this printing method
// TODO: Have it print diff color for diff distros
void Printer::Print() {
	SystemInfo si; 
	Configuration cfg;
	Icons icon;
	// Initalize all the values we need
	si.InitializeDistroID();
	(cfg.ParseConfigFile() == 1 ? si.Initialize(true) : si.Initialize(false));
	int firstBar = Configuration::width / 2 - 1;
	bool addedWhiteSpace = false;
	int bar = Configuration::width + firstBar;
	auto RepeatString = [&](std::string_view str, int n) { std::string result; for(int i = 0; i < n; i++) { result += str; } return result; };
	auto AddWhiteSpace = [&](std::string_view str, int n) { std::string result; for(size_t i = 0; i < str.size(); ++i) { result += str[i]; if(i != str.size() - 1 && n > 0) { addedWhiteSpace = true; } } return result; };
	// Output
	std::cout << C::B_BLUE << (Configuration::showAscii ? si.logo : "") << C::NC << std::endl;
	if(!icon.showNothing) { 
		std::cout << "  ╭" << std::setfill(' ') << RepeatString("──", Configuration::width) << "╮" << std::endl;
		if(icon.showUsername) {
			std::cout << "  │ " << C::YELLOW << icon.iconUser << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("user  ", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ "
				<< C::RED << si.user << C::NC << std::endl;
		}
		if(icon.showHostname) { 
			std::cout << "  │ " << C::YELLOW << icon.iconHname << C::NC;
			std::cout << std::setw(firstBar) << " " <<  AddWhiteSpace("hname ", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) <<  "│ " 
				<< C::YELLOW << si.hostname << C::NC << std::endl;
		}
		if(icon.showDistro) { 
			std::cout << "  │ " << C::G << icon.iconDistro << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("distro", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::G << si.distro << C::NC << std::endl;
		}
		if(icon.showKernel) { 
			std::cout << "  │ " << C::BRIGHT_BLUE << icon.iconKernel << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("kernel", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::BRIGHT_BLUE << si.kernel << C::NC << std::endl;
		}
		if(icon.showUptime) {
			std::cout << "  │ " << C::BLUE << icon.iconUptime << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("uptime", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::BLUE << si.uptime << C::NC << std::endl;
		}
		if(icon.showShell) { 
			std::cout << "  │ " << C::PURPLE <<  icon.iconShell << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("shell ", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::PURPLE << si.shell << C::NC << std::endl;
		}
		if(icon.showDeWm) { 
			std::cout << "  │ " << C::G << icon.iconDeWm << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("de/wm ", Configuration::width - 6)<< std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::G << si.de << C::NC << std::endl;
		}
		if(icon.showPkg) { 
			std::cout << "  │ " << C::RED << icon.iconPkgs << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("pkgs  ", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::RED << si.packageCount << C::NC << std::endl;
		}
		if(icon.showMemory) { 
			std::cout << "  │ " << C::YELLOW << icon.iconMemory << C::NC;
			std::cout << std::setw(firstBar) << " " << AddWhiteSpace("memory", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< C::YELLOW << si.memory << C::NC << std::endl;
		}
		if(icon.showColors) { 
			if(icon.showUsername || icon.showHostname || icon.showDistro || icon.showKernel || icon.showUptime || icon.showShell || icon.showDeWm || icon.showPkg || icon.showMemory) {
				std::cout << "  ├" << std::setfill(' ') << RepeatString("──", Configuration::width) << "┤" << std::endl;
			}
			std::cout << "  │ " << icon.iconColors
				<< std::setw(firstBar) << " " << AddWhiteSpace("colors", Configuration::width - 6) << std::setw((Configuration::width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
				<< PrintColors() << std::endl;
		}
		std::cout << "  ╰" << std::setfill(' ') << RepeatString("──", Configuration::width) << "╯" << std::endl;
	} 
} // ends Print()

std::string Printer::PrintColors() {
	Icons icon;
	std::ostringstream oss;
	oss << icon.iconColorSwatches << C::RED << " " << icon.iconColorSwatches << C::YELLOW << " " << icon.iconColorSwatches << C::G << " " << icon.iconColorSwatches << C::BRIGHT_BLUE << " " << icon.iconColorSwatches << C::BLUE << " " << icon.iconColorSwatches << C::PURPLE << " " << icon.iconColorSwatches << C::NC;
	return oss.str();
} // ends PrintColors()
