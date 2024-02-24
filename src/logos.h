#pragma once
#include <unordered_map>
#include <string>

struct Logos {
	static std::string GetLogos(const std::string& id);
	static std::unordered_map<std::string, std::string> ReadAsciiArt(const std::string& filename);
};
