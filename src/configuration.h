#pragma once
#include <string>

struct Configuration {
	static std::string s_configFile, s_tmpDistro, s_filename;
	static bool s_showAscii, s_distroSuppliedFromCli, s_widthSupplied, s_noNerdFonts;
	static int s_width;
	size_t ParseConfigFile();
	std::string GetConfigPath();
};
