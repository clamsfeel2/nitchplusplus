#include "cli-processor.h"
#include "colors.hpp"
#include <iostream>
#include <sstream>
#include <stack>

int CliProcessor::ProcessCliArgs(int argC, char** argV) {
	std::stack<std::string> args;
	for(int i = 1; i < argC; ++i) {
		args.emplace(argV[i]);
	} 
	if(args.empty()) {
		return 1;
	}
	std::string flag = args.top();
	args.pop();
	if(flag == "-na" || flag == "--noascii") {
		Configuration::noAscii = false;
		return 1;
	} else if(flag == "-h" || flag == "--help") {
		PrintHelp();
		return 0;
	} else {
		std::ostringstream oss;
		oss << C::B_RED << "ERROR: " << C::NC << "incorrect usage \"" << flag << "\"" << std::endl << C::B_WHITE << "Run 'fetchpp --help' to see a list of legal commands.";
		throw std::invalid_argument(oss.str());
	}
	return 1;
}

int CliProcessor::PrintHelp() {
	std::cout <<  "-na, --noascii" <<  C::NC  <<
		std::endl << "     will run fetch++ without displaying ascii art." << std::endl;
	std::cout << std::endl;
	std::cout <<  "-h, --help" <<  C::NC  <<
		std::endl << "     display help." << std::endl;
	std::cout << std::endl;
	return 0;
} // ends PrintHelp()

