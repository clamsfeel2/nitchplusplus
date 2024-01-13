#include <string>
#include <iostream>
#include <unordered_map>

struct Configuration {
	static std::string configFile;
	static bool showAscii;
	static bool widthSupplied;
	std::string envConfigFile = "FETCHPP_CONFIG_FILE";
	std::string filename;
	size_t ParseConfigFile();
	std::string GetConfigPath();
	static int width;
};
