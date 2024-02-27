#include "cli-processor.h"
#include "colors.hpp"
#include <iostream>
#include <sstream>
#include <queue>
#include <cctype>

int CliProcessor::ProcessCliArgs(int argC, char* argV[]) {
	std::queue<std::string> args;
	for(int i = 1; i < argC; i++) {
		args.emplace(argV[i]);
	} 
	while(!args.empty()) { 
		std::ostringstream oss;
		std::string flag(args.front());
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
			} else if(std::stoi(args.front()) < 5) {
				oss << C::B_RED << "ERROR: " << C::NC << "the width value MUST be greater than 5 or else the display breaks :(";
				throw std::invalid_argument(oss.str());
			} else {
				Configuration::width = std::stoi(args.front());
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
