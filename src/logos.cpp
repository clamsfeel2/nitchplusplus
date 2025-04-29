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
    const auto& art = LoadAsciiArt();
    return art.find(id) != art.end();
}

std::string Logos::GetAsciiFilePath() {
    if(!Configuration::configFile.empty()) return std::filesystem::path(Configuration::configFile).parent_path() / ".ascii.txt";
    if(char* xdh = std::getenv("XDG_CONFIG_HOME")) return std::filesystem::path(xdh) / "nitch++" / ".ascii.txt";
    if(char* home = std::getenv("HOME")) return std::filesystem::path(home) / ".config" / "nitch++" / ".ascii.txt";
    throw std::runtime_error("Could not determine ascii path: HOME and XDG_CONFIG_HOME are both unset and no path found in config file.");
}

const std::unordered_map<std::string, std::string>& Logos::LoadAsciiArt() {
    static const auto artMap = []() {
        std::unordered_map<std::string, std::string> m;
        std::ifstream file(GetAsciiFilePath());
        if(!file) {
            std::cerr << "Could not open ASCII art file: " << GetAsciiFilePath() << std::endl;
            return m;
        }
        std::string line, key;
        while(std::getline(file, line)) {
            if(line.empty()) continue;
            if(!std::isspace(static_cast<unsigned char>(line.front()))) {
                key = line;
                m[key].clear();
            } else if(!key.empty()) {
                m[key] += line + '\n';
            }
        }
        return m;
    }();
    return artMap;
}
