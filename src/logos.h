#pragma once
#include <string>
#include <unordered_map>

class Logos {
public:
    static const std::string& GetLogos(const std::string& id);
    static bool IsValidDistro(const std::string& id);
private:
    static const std::unordered_map<std::string, std::string>& LoadAsciiArt();
    static std::string GetAsciiFilePath();
};
