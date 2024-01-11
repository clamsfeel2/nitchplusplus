#include "printer.h"
#include "configuration.h"
#include "system-info.h"
#include "icons.h"
#include "colors.hpp"
#include <ios>
#include <sstream>

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

	// Output
	std::cout << C::B_BLUE << (Configuration::noAscii ? si.logo : "") << C::NC << std::endl;
	if(!icon.showNothing) { 
	std::cout << "  ╭────────────╮" << std::endl;
	if(icon.showUsername) { 
	std::cout << "  │ " << C::YELLOW << icon.iconUser << C::NC;
	std::cout << "  user    │ "
		<< C::RED << si.user << C::NC << std::endl;
	}
	if(icon.showHostname) { 
	std::cout << "  │ " << C::YELLOW << icon.iconHname << C::NC;
	std::cout <<	"  hname   │ " << C::YELLOW << si.hostname << C::NC << std::endl;
	}
	if(icon.showDistro) { 
	std::cout << "  │ " << C::G << icon.iconDistro << C::NC;
	std::cout << "  distro  │ " << C::G << si.distro << C::NC << std::endl;
	}
	if(icon.showKernel) { 
	std::cout << "  │ " << C::BRIGHT_BLUE << icon.iconKernel << C::NC << "  kernel  │ " << C::BRIGHT_BLUE << si.kernel << C::NC << std::endl;
	}
	if(icon.showUptime) {
	std::cout << "  │ " << C::BLUE << icon.iconUptime << C::NC << "  uptime  │ " << C::BLUE << si.uptime << C::NC << std::endl;
	}
	if(icon.showShell) { 
	std::cout << "  │ " << C::PURPLE <<  icon.iconShell << C::NC << "  shell   │ " << C::PURPLE << si.shell << C::NC << std::endl;
	}
	if(icon.showDeWm) { 
	std::cout << "  │ " << C::G << icon.iconDeWm << C::NC;
	std::cout << "  de/wm   │ " << C::G << si.de << " | " << si.wm << C::NC << std::endl;
	}
	if(icon.showPkg) { 
	std::cout << "  │ " << C::RED << icon.iconPkgs << C::NC << "  pkgs    │ " << C::RED << si.packageCount << C::NC << std::endl;
	}
	if(icon.showMemory) { 
	std::cout << "  │ " << C::YELLOW << icon.iconMemory << C::NC << "  memory  │ " << C::YELLOW << si.memory << C::NC << std::endl;
	}
	if(icon.showColors) { 
		if(icon.showUsername || icon.showHostname || icon.showDistro || icon.showKernel || icon.showUptime || icon.showShell || icon.showDeWm || icon.showPkg || icon.showMemory) {
			std::cout << "  ├────────────┤" << std::endl;
		}
	std::cout << "  │ " << icon.iconColors << "  colors  │" << " " << PrintColors() << std::endl;
	}
	std::cout << "  ╰────────────╯" << std::endl;
	} 
} // ends Print()

std::string Printer::PrintColors() {
	std::string dot = "";
	std::ostringstream oss;
	oss << dot << C::RED << " " << dot << C::YELLOW << " " << dot << C::G << " " << dot << C::BRIGHT_BLUE << " " << dot << C::BLUE << " " << dot << C::PURPLE << " " << dot << C::NC;
	return oss.str();
} // ends PrintColors()
