#pragma once
#include <string>

namespace CliProcessor {
	int ProcessCliArgs(int argC, char* argV[]);
	inline const char* PrintHelp() { return
		"-na, --noascii" "\n     will run fetch++ without displaying ascii art.\n\n"
			"-d, --distro [distro name]"  "\n     will display ascii art of specified distro.\n\n"
			"-w, --width [>=5]"  "\n     will change the width of the output box with specified value.\n\n"
			"-c, --clear"  "\n     will clear the screen before running fetch++.\n";
	}
};

