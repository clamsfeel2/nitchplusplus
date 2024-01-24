#include "configuration.h"
#include "system-info.h"
#include "colors.hpp"
#include "icons.h"
#include "logos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem> // for std::filesystem
#include <toml++/toml.hpp> // for tomlplusplus

bool Configuration::showAscii = true;
bool Configuration::widthSupplied = false;
std::string Configuration::configFile = " ";
std::string Configuration::tmpDistro = "";
bool Configuration::distroSuppliedFromCli;
int Configuration::width = 6;

std::string Configuration::GetConfigPath() {
	const char* envValue = std::getenv(envConfigFile.c_str());
	// Check if the environment variable is set
	if(envValue != nullptr) {
		// If set -- set resultPath to env variable path
		std::string resultPath = std::string(envValue);
		// Extract the parent path from the full file path and casting resultPath as filesystem type to use parent_path
		std::filesystem::path parentPath = std::filesystem::path(resultPath).parent_path();
		// Check if the directory exists in env path, and create it if not
		if(!std::filesystem::exists(parentPath)) {
			if(!std::filesystem::create_directories(parentPath)) {
				const std::string errorMessage = "Error: Could not create config directory: ";
				std::ostringstream ossString;
				ossString << errorMessage << parentPath;
				const std::string finalMessage = ossString.str();
				throw std::invalid_argument(finalMessage);
			} 
		} 

		// Check if the config file exists, and create it if not
		configFile = resultPath;
		std::string configFilePath = std::string(std::getenv("HOME")) + "/.config/fetchplusplus/config.toml";

		// Return resulting filename
		return configFilePath;
	} else {
		// If env var not set -- create directory at $HOME/.config/tinytask
		std::string defaultDirectory = std::string(std::getenv("HOME")) + "/.config/fetchplusplus/";
		std::string configFilename = defaultDirectory + "config.toml";

		// Check if the directory exists -- and create it if not
		if(!std::filesystem::exists(defaultDirectory)) {
			if(!std::filesystem::create_directories(defaultDirectory)) {
				throw std::invalid_argument("Error: Could not create directory: " + defaultDirectory);
			} 
		} 
		return configFilename;
	}  
} // ends GetConfigPath()

// TODO: ADD VALIDATION TO MAKE SURE CONFIGURATION OPTIONS USER HAS PUT ARE LEGAL
size_t Configuration::ParseConfigFile() {
	Icons icon;
	configFile = GetConfigPath();
	if(!std::filesystem::exists(configFile)) {
		return 2;
	}
	// Parse toml file
	toml::table parser = toml::parse_file(configFile);
	// Icons
	icon.iconUser = parser["modules"]["username"][0].value_or("");
	icon.showUsername = parser["modules"]["username"][1].value_or(true);

	icon.iconHname = parser["modules"]["hostname"][0].value_or("");
	icon.showHostname = parser["modules"]["hostname"][1].value_or(true);

	// Lambda uses distroID to find matching icon in distroIconMap and returns penguin if distro not found
	icon.iconDistro = parser["modules"]["distro"][0].value_or([&]() -> std::string { auto it = Icons::distroIconMap.find(SystemInfo::distroID); return (it != Icons::distroIconMap.end()) ? it->second : "󰻀"; }());
	icon.showDistro = parser["modules"]["distro"][1].value_or(true);

	icon.iconKernel = parser["modules"]["kernel"][0].value_or("󰌢");
	icon.showKernel = parser["modules"]["kernel"][1].value_or(true);

	icon.iconUptime = parser["modules"]["uptime"][0].value_or("");
	icon.showUptime = parser["modules"]["uptime"][1].value_or(true);

	icon.iconShell = parser["modules"]["shell"][0].value_or("");
	icon.showShell = parser["modules"]["shell"][1].value_or(true);

	icon.iconDeWm = parser["modules"]["dewm"][0].value_or("");
	icon.showDeWm = parser["modules"]["dewm"][1].value_or(false);

	icon.iconPkgs = parser["modules"]["pkgs"][0].value_or("󰏖");
	icon.showPkg = parser["modules"]["pkgs"][1].value_or(true);

	icon.iconMemory = parser["modules"]["memory"][0].value_or("󰍛");
	icon.showMemory = parser["modules"]["memory"][1].value_or(true);

	icon.iconColors = parser["modules"]["colors"][0].value_or("");
	icon.iconColorSwatches = (parser["modules"]["colors"][1] == "" ? "" : parser["modules"]["colors"][1].value_or("")); // shows dot for swatch if value in toml file is empty or absent
	icon.showColors = parser["modules"]["colors"][2].value_or(true);

	// Checking if all values are false to print nothing.
	icon.showNothing = (!icon.showUsername && !icon.showHostname && !icon.showDistro && !icon.showKernel && !icon.showUptime && !icon.showShell && !icon.showDeWm && !icon.showPkg && !icon.showMemory && !icon.showColors) ? true : false;
	// General
	if(showAscii) { 
		Configuration::showAscii = parser["general"]["show_ascii"].value_or(false);
	}
	if(!widthSupplied) { 
		Configuration::width = parser["general"]["width"].value_or(6);
		if(Configuration::width < 5) {
			std::ostringstream oss;
			oss << C::B_RED << "ERROR: " << C::NC << "width value in your config file MUST be greater than 5 or else my display breaks :(";
			throw std::invalid_argument(oss.str());
		}
	}
	std::string tmp = parser["general"]["ascii_distro"].value_or("");
	if(Configuration::distroSuppliedFromCli) {
		SystemInfo::logo = Logos::GetLogos(tmpDistro);
	} else { 
		return 2;
	}
	return 0;
} // ends ParseConfigFile()
