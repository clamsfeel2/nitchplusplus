#include <cstring>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "cli_processor.h"
#include "configuration.h"
#include "colors.hpp"
#include "logos.h"

int CliProcessor::ProcessCliArgs(int argc, char* argv[]) {
    auto error = [&](std::string_view msg) -> void { throw std::invalid_argument(std::string(C::B_RED) + "ERROR: " + C::NC + msg.data()); };
    auto to_lower = [](std::string& s) -> void { std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); }); };
    int opt;

    while((opt = getopt_long(argc, argv, "nahw:cd:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'n':
                Configuration::s_noNerdFonts = true;
                break;

            case 'a':
                Configuration::s_showAscii = false;
                break;

            case 'h':
                std::cout << PrintHelp();
                return 0;

            case 'w': {
                const int MIN_WIDTH = 6;
                if(!optarg || !std::isdigit(optarg[0])) error("Invalid width value.");
                int w = std::stoi(optarg);
                if(w < MIN_WIDTH) error("width must be >= " + std::to_string(MIN_WIDTH));
                Configuration::s_width = w;
                Configuration::s_widthSupplied = true;
                break;
            }

            case 'c':
                std::cout << "\033c";
                break;

            case 'd': {
                if(!optarg) error("Missing distro name.");
                std::string d(optarg);
                to_lower(d);
                if(!Logos::IsValidDistro(d.c_str())) error("Distro not found.");
                Configuration::s_distroSuppliedFromCli = true;
                Configuration::s_tmpDistro = std::move(d);
                break;
            }

            default:
                error("Incorrect usage.");
        }
    }
    return 1;
}
