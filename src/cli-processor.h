#pragma once
#include <string>
#include <getopt.h>

namespace CliProcessor {
	int ProcessCliArgs(int argC, char* argV[]);
	inline struct option long_options[] = {
		{"noascii", no_argument, 0, 'n'},
		{"help", no_argument, 0, 'h'},
		{"width", required_argument, 0, 'w'},
		{"clear", no_argument, 0, 'c'},
		{"distro", required_argument, 0, 'd'},
		{0, 0, 0, 0}
	};
	inline const char* PrintHelp() { return
		"-n, --noascii" "\n     will run fetch++ without displaying ascii art.\n\n"
			"-d, --distro [distro name]"  "\n     will display ascii art of specified distro.\n\n"
			"-w, --width [>=5]"  "\n     will change the width of the output box with specified value.\n\n"
			"-c, --clear"  "\n     will clear the screen before running fetch++.\n";
	}
};

