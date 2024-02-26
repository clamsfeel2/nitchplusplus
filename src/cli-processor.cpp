#include "cli-processor.h"
#include "colors.hpp"
#include <iostream>
#include <sstream>
#include <queue>
#include <cctype>

int CliProcessor::ProcessCliArgs(int argC, char** argV) {
	std::queue<std::string> args;
	std::ostringstream oss;
	for(int i = 1; i < argC; ++i) {
		args.emplace(argV[i]);
	} 
	if(args.empty()) {
		return 1;
	}
	while(!args.empty()) { 
		std::string flag = args.front();
		args.pop();
		if(flag == "-na" || flag == "--noascii") {
			Configuration::showAscii = false;
		} else if(flag == "-h" || flag == "--help") {
			std::cout << PrintHelp();
			return 0;
		} else if(flag == "-w" || flag == "--width") {
			if(args.empty() || !isdigit(args.front()[0])) {
				oss << C::B_RED << "ERROR: " << C::NC << "invalid number of arguments"<< std::endl << C::B_WHITE << "Run 'fetchpp --help' to see a list of legal commands.";
				throw std::invalid_argument(oss.str());
			} else {
				Configuration::width = std::stoi(args.front());
			}
			if(std::stoi(args.front()) < 5) {
				oss << C::B_RED << "ERROR: " << C::NC << "the width value MUST be greater than 5 or else my display breaks :(";
				throw std::invalid_argument(oss.str());
			}
			Configuration::width = std::stoi(args.front());
			Configuration::widthSupplied = true;
			args.pop();
		} else if(flag == "-d" || flag == "--distro") {
			if(args.empty()) {
				oss << C::B_RED << "ERROR: " << C::NC << "invalid number of arguments"<< std::endl << C::B_WHITE << "Run 'fetchpp --help' to see a list of legal commands.";
				throw std::invalid_argument(oss.str());
			} else {
				Configuration::distroSuppliedFromCli = true;
				Configuration::tmpDistro = args.front();
				args.pop();
			}
		} else {
			oss << C::B_RED << "ERROR: " << C::NC << "incorrect usage \"" << flag << "\"" << std::endl << C::B_WHITE << "Run 'fetchpp --help' to see a list of legal commands.";
			throw std::invalid_argument(oss.str());
		}
	}
	return 1;
}

constexpr const char* CliProcessor::PrintHelp() {
	return
		"-na, --noascii" "\n     will run fetch++ without displaying ascii art.\n\n"
		"-d, --distro [distro name]"  "\n     will display ascii art of specified distro.\n\n"
		"-w, --width [>=5]"  "\n     will change the width of the output box with specified value.\n";
} // ends PrintHelp()
