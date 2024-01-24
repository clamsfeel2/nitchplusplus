#include "cli-processor.h"
#include "colors.hpp"
#include <iostream>
#include <sstream>
#include <queue>

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
			PrintHelp();
			return 0;
		} else if(flag == "-w" || flag == "--width") {
			if(args.empty()) {
				oss << C::B_RED << "ERROR: " << C::NC << "invalid number of arguments"<< std::endl << C::B_WHITE << "Run 'fetchpp --help' to see a list of legal commands.";
				throw std::invalid_argument(oss.str());
			} else {
				Configuration::width = std::stoi(args.front());
			}
			if(std::stoi(args.front()) < 5) {
				oss << C::B_RED << "ERROR: " << C::NC << "width value in your config file MUST be greater than 5 or else my display breaks :(";
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

int CliProcessor::PrintHelp() {
	std::cout <<  "-na, --noascii" <<  C::NC  <<
		std::endl << "     will run fetch++ without displaying ascii art." << std::endl;
	std::cout << std::endl;
	std::cout <<  "-d, --distro [distro name]" <<  C::NC  <<
		std::endl << "     will display ascii art of specified distro." << std::endl;
	std::cout << std::endl;
	std::cout <<  "-w, --width [>=5]" <<  C::NC  <<
		std::endl << "     will change the width of the output box with specified value." << std::endl;
	std::cout << std::endl;
	return 0;
} // ends PrintHelp()

