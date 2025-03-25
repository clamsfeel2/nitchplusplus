#include <cstring>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <sstream>
#include "cli_processor.h"
#include "configuration.h"
#include "colors.hpp"
#include "logos.h"

int CliProcessor::ProcessCliArgs(int argc, char* argv[]) {
    std::ostringstream oss;
    int opt = 0;

    while((opt = getopt_long(argc, argv, "nahw:cd:", long_options, NULL)) != -1) {
        switch(opt) {
            case 'n':
                Configuration::noNerdFonts = true;
                break;
            case 'a':
                Configuration::showAscii = false;
                break;
            case 'h':
                std::cout << PrintHelp();
                return 0;
            case 'w': {
                if(optarg == nullptr) {
                    oss << C::B_RED << "ERROR: " << C::NC << "Invalid number of arguments given.";
                    throw std::invalid_argument(oss.str());
                }
                if(!std::isdigit(optarg[0])) {
                    oss << C::B_RED << "ERROR: " << C::NC << "Invalid width value.";
                    throw std::invalid_argument(oss.str());
                }
                int width = std::stoi(optarg);
                if(width < 5) {
                    oss << C::B_RED << "ERROR: " << C::NC << "Invalid width value.";
                    throw std::invalid_argument(oss.str());
                }
                Configuration::width = width;
                Configuration::widthSupplied = true;
                break;
            }
            case 'c':
                std::cout << "\033c" << std::endl;
                break;
            case 'd':
                Configuration::distroSuppliedFromCli = true;
                std::transform(optarg, optarg + strlen(optarg), optarg, [](unsigned char c) { return std::tolower(c); });
				if(!Logos::IsValidDistro(optarg)) {
					oss << C::B_RED << "ERROR: " << C::NC << "Distro not found.";
                    throw std::invalid_argument(oss.str());
				}
                Configuration::tmpDistro = optarg;
                break;
            default:
                oss << C::B_RED << "ERROR: " << C::NC << "Incorrect usage.";
                throw std::invalid_argument(oss.str());
        }
    }
    return 1;
}
