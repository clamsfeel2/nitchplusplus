#include "printer.h"
#include "configuration.h"
#include "system-info.h"
#include "icons.h"
#include "colors.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

// FIXME: Organize this printing method
// TODO: Have it print diff color for diff distros
void Printer::Print() {
	SystemInfo si; 
	Configuration cfg;
	Icons icon;
	// Initalize all the values we need
	if((cfg.ParseConfigFile() == 2)) {
		si.Initialize(true);
	}
	si.Initialize(false);
	int width = Configuration::width;
	int firstBar = width / 2 - 1;
	int bar = width + firstBar;
	// Lambda to repeat a string n times in order to add more _ when changing the width 
	auto RepeatString = [&](const std::string& str, int n) { std::string result; for(int i = 0; i < n; i++) { result += str; } return result; };
	// Output
	std::cout << C::B_BLUE << (Configuration::showAscii ? si.logo : "") << C::NC << std::endl;
	if(!icon.showNothing) { 
	 std::cout << "  ╭" << std::setfill(' ') << RepeatString("──", width) << "╮" << std::endl;
	 if(icon.showUsername) {
		 std::cout << "  │ " << C::YELLOW << icon.iconUser << C::NC;
		 std::cout << std::setw(firstBar) << " " << "user" << std::setw((width % 2 == 0 ? bar : bar + 1)) <<  "│ "
			 << C::RED << si.user << C::NC << std::endl;
	 }
	 if(icon.showHostname) { 
	std::cout << "  │ " << C::YELLOW << icon.iconHname << C::NC;
	std::cout << std::setw(firstBar) << " " <<  "hname" << std::setw((width % 2 == 0 ? bar - 1 : bar)) <<  "│ " 
		<< C::YELLOW << si.hostname << C::NC << std::endl;
	}
	if(icon.showDistro) { 
	std::cout << "  │ " << C::G << icon.iconDistro << C::NC;
	std::cout << std::setw(firstBar) << " " << "distro" << std::setw((width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
		<< C::G << si.distro << C::NC << std::endl;
	}
	if(icon.showKernel) { 
	std::cout << "  │ " << C::BRIGHT_BLUE << icon.iconKernel << C::NC;
	std::cout << std::setw(firstBar) << " " << "kernel" << std::setw((width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
		<< C::BRIGHT_BLUE << si.kernel << C::NC << std::endl;
	}
	if(icon.showUptime) {
	std::cout << "  │ " << C::BLUE << icon.iconUptime << C::NC;
	std::cout << std::setw(firstBar) << " " << "uptime" << std::setw((width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
		<< C::BLUE << si.uptime << C::NC << std::endl;
	}
	if(icon.showShell) { 
	std::cout << "  │ " << C::PURPLE <<  icon.iconShell << C::NC;
	std::cout << std::setw(firstBar) << " " << "shell" << std::setw((width % 2 == 0 ? bar - 1 : bar)) << "│ " 
		<< C::PURPLE << si.shell << C::NC << std::endl;
	}
	if(icon.showDeWm) { 
	std::cout << "  │ " << C::G << icon.iconDeWm << C::NC;
	std::cout << std::setw(firstBar) << " " << "de/wm" << std::setw((width % 2 == 0 ? bar - 1 : bar)) << "│ " 
		<< C::G << si.de << " | " << si.wm << C::NC << std::endl;
	}
	if(icon.showPkg) { 
	std::cout << "  │ " << C::RED << icon.iconPkgs << C::NC;
	std::cout << std::setw(firstBar) << " " << "pkgs" << std::setw((width % 2 == 0 ? bar : bar + 1)) << "│ " 
		<< C::RED << si.packageCount << C::NC << std::endl;
	}
	if(icon.showMemory) { 
	std::cout << "  │ " << C::YELLOW << icon.iconMemory << C::NC;
	std::cout << std::setw(firstBar) << " " << "memory" << std::setw((width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
		<< C::YELLOW << si.memory << C::NC << std::endl;
	}
	if(icon.showColors) { 
		if(icon.showUsername || icon.showHostname || icon.showDistro || icon.showKernel || icon.showUptime || icon.showShell || icon.showDeWm || icon.showPkg || icon.showMemory) {
			std::cout << "  ├" << std::setfill(' ') << RepeatString("──", width) << "┤" << std::endl;
		}
	std::cout << "  │ " << icon.iconColors
		<< std::setw(firstBar) << " " << "colors" << std::setw((width % 2 == 0 ? bar - 2 : bar - 1)) << "│ " 
		<< PrintColors() << std::endl;
	}
	std::cout << "  ╰" << std::setfill(' ') << RepeatString("──", width) << "╯" << std::endl;

	} 
} // ends Print()

std::string Printer::PrintColors() {
	Icons icon;
	std::ostringstream oss;
	oss << icon.iconColorSwatches << C::RED << " " << icon.iconColorSwatches << C::YELLOW << " " << icon.iconColorSwatches << C::G << " " << icon.iconColorSwatches << C::BRIGHT_BLUE << " " << icon.iconColorSwatches << C::BLUE << " " << icon.iconColorSwatches << C::PURPLE << " " << icon.iconColorSwatches << C::NC;
	return oss.str();
} // ends PrintColors()
