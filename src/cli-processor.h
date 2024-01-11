#pragma once
#include "configuration.h"
#include <string>

struct CliProcessor {
	static int PrintHelp();
	static int ProcessCliArgs(int argC, char** argV);
};

