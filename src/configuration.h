#pragma once
#include <string>
#include <iostream>
#include <unordered_map>

struct Configuration {
	static std::string configFile, tmpDistro, filename;
	static bool showAscii, distroSuppliedFromCli, widthSupplied;
	static int width;
	constexpr static const char envConfigFile[] = "FETCHPP_CONFIG_FILE";
	size_t ParseConfigFile();
	std::string GetConfigPath();
};
