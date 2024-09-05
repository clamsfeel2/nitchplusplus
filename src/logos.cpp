#include "logos.h"
#include "configuration.h"
#include <iostream>
#include <fstream>
#include <filesystem>

std::string Logos::GetLogos(const std::string& id) {
	// Get parent path of config file to get the ascii art and casting it to use parent_path()
	std::string asciiFile(static_cast<std::filesystem::path>(Configuration::configFile).parent_path());
	asciiFile += "/.ascii.txt";
	std::unordered_map<std::string, std::string> logos = Logos::ReadAsciiArt(asciiFile);
	auto it = logos.find(id);
	if(it != logos.end()) {
		return it->second;
	} 
	return logos["default"]; 
} // ends GetLogos()

std::unordered_map<std::string, std::string> Logos::ReadAsciiArt(const std::string& filename) {
	std::ifstream file(filename);
	std::unordered_map<std::string, std::string> asciiMap;
	if(file.is_open()) {
		std::string line;
		std::string key;
		while(std::getline(file, line)) {
			// Check if the line is empty if so new key time
			if(line.empty()) {
				continue;
			}
			// Check if the line starts with whitespace meaning art!!
			if(std::isspace(line[0])) {
				asciiMap[key] += line + '\n';
			} else {
				key = line;
				// Read the next four lines as ascii art
				for(int i = 0; i < 4 && std::getline(file, line); i++) {
					asciiMap[key] += line + '\n';
				}
			}
		}
		file.close();
	} else {
		std::cerr << "Error opening ascii art file." << std::endl;
	}
	return asciiMap;
} // ends ReadAsciiArt()
