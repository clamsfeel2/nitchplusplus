#include "logos.h"
#include "configuration.h"
#include <filesystem>
#include <fstream>
#include <iostream>

const std::string& Logos::GetLogos(const std::string& id) {
    const std::unordered_map<std::string, std::string>& art = LoadAsciiArt();
    auto it = art.find(id);
    if(it != art.end()) return it->second;
    static const std::string empty;
    auto def = art.find("default");
    return(def != art.end()) ? def->second : empty;
}

bool Logos::IsValidDistro(const std::string& id) {
    const std::unordered_map<std::string, std::string>& art = LoadAsciiArt();
    return art.find(id) != art.end();
}

std::string Logos::GetAsciiFilePath() {
    if(!Configuration::s_configFile.empty()) return std::filesystem::path(Configuration::s_configFile).parent_path() / ".ascii.txt";
    if(char* xdh = std::getenv("XDG_CONFIG_HOME")) return std::filesystem::path(xdh) / "nitch++" / ".ascii.txt";
    if(char* home = std::getenv("HOME")) return std::filesystem::path(home) / ".config" / "nitch++" / ".ascii.txt";
    throw std::runtime_error("Could not determine ascii path: HOME and XDG_CONFIG_HOME are both unset and there is no path found in config file.");
}

const std::unordered_map<std::string, std::string>& Logos::LoadAsciiArt() {
    static std::unordered_map<std::string, std::string> artMap;
    static bool initialized = false;

    if(!initialized) {
        initialized = true;
        std::string path = GetAsciiFilePath();
        std::ifstream file(path);
         if(!file) {
            std::cerr << "Could not open ASCII art file: " << path << std::endl;
            return artMap;
        }
        std::string line, key;
        while(std::getline(file, line)) {
            if(line.empty()) continue;
            if(!std::isspace(static_cast<unsigned char>(line.front()))) {
                key = std::move(line);
                artMap[key].clear();
            } else if(!key.empty()) {
                artMap[key].append(line).push_back('\n');
            }
        }
    }
    return artMap;
}
