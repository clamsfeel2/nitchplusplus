#pragma once
#include <string>

struct Configuration {
	static std::string configFile, tmpDistro, filename;
	static bool showAscii, distroSuppliedFromCli, widthSupplied, noNerdFonts;
	static int width;
	size_t ParseConfigFile();
	std::string GetConfigPath();
};
