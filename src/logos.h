#pragma once
#include "system-info.h"
#include <unordered_map>

struct Logos {
	static std::string GetLogos(const std::string& id);
	static std::unordered_map<std::string, std::string> ReadAsciiArt(const std::string& filename);
};
