#include "logos.h"
#include "configuration.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <cctype>

// Helper: determines the ascii art file path.
static std::string GetAsciiFilePath() {
    if(!Configuration::configFile.empty()) {
        return std::filesystem::path(Configuration::configFile).parent_path().string() + "/.ascii.txt";
    }
	if(const char* home = std::getenv("HOME")) {
		return std::filesystem::path(home) / ".config" / "nitch++" / ".ascii.txt";
	}
    throw std::runtime_error("HOME environment variable is not set.");
}

std::string Logos::GetLogos(const std::string& id) {
    std::string asciiFile = GetAsciiFilePath();
    auto logos = Logos::ReadAsciiArt(asciiFile);
    auto it = logos.find(id);
    return (it != logos.end()) ? it->second : logos["default"];
}

std::unordered_map<std::string, std::string> Logos::ReadAsciiArt(const std::string& filename) {
    std::ifstream file(filename);
    std::unordered_map<std::string, std::string> asciiMap;
    if(file.is_open()) {
        std::string line, key;
        while(std::getline(file, line)) {
            if(line.empty())
                continue;
            if(std::isspace(line[0]))
                asciiMap[key] += line + '\n';
            else {
                key = line;
                // Read the next four lines as ascii art.
                for (int i = 0; i < 4 && std::getline(file, line); ++i)
                    asciiMap[key] += line + '\n';
            }
        }
        file.close();
    } else {
        std::cerr << "Error opening ascii art file: " << filename << std::endl;
    }
    return asciiMap;
}

// New function: checks if the given distro exists in the ascii art file.
bool Logos::IsValidDistro(const std::string& distro) {
    std::string asciiFile = GetAsciiFilePath();
    auto logos = Logos::ReadAsciiArt(asciiFile);
    return logos.find(distro) != logos.end();
}
