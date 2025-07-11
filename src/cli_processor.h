#pragma once
#include <getopt.h>

namespace CliProcessor {
	int ProcessCliArgs(int argC, char* argV[]);
	inline struct option long_options[] = {
		{"noascii", no_argument, 0, 'a'},
		{"nonerd", no_argument, 0, 'n'},
		{"help", no_argument, 0, 'h'},
		{"width", required_argument, 0, 'w'},
		{"clear", no_argument, 0, 'c'},
		{"distro", required_argument, 0, 'd'},
        {"version",  no_argument,       0, 'v'},
		{0, 0, 0, 0}
	};
	inline const char* PrintHelp() {
    return
        "usage: nitch++ [-h] [-a] [-n] [-c] [-v] [-w WIDTH] [-d DISTRO]\n\n"
        "nitch++: a fast system information fetch tool.\n\n"
		"-a, --noascii" "\n     \e[2mrun nitch++ without displaying ascii art.\e[0m\n\n"
			"-n, --nonerd"  "\n     \e[2mdo not use nerd fonts.\e[0m\n\n"
			"-d, --distro [distro name]"  "\n     \e[2mdisplay ascii art of specified distro.\e[0m\n\n"
			"-w, --width [>=6]"  "\n     \e[2mchange the width of the output box with specified value.\e[0m\n\n"
			"-c, --clear"  "\n     \e[2mclear the screen before running nitch++.\e[0m\n\n"
			"-v, --version"  "\n     \e[2mshow nitch++ version.\e[0m\n";
	}
    inline const char* PrintVersion() {
        return "nitch++ v1.0.0";
    }
};
