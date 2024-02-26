#pragma once
#include "configuration.h"
#include <string>

namespace CliProcessor {
	constexpr const char* PrintHelp();
	int ProcessCliArgs(int argC, char** argV);
};

