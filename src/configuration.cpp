#include "configuration.h"
#include "system_info.h"
#include "colors.hpp"
#include "icons.h"
#include "logos.h"
#include <filesystem>
#include <toml++/toml.hpp>
#include <algorithm>
#include <array>

std::string Configuration::s_configFile     = "";
std::string Configuration::s_tmpDistro      = "";
int Configuration::s_width                  = 6;
std::string Configuration::s_asciiColor     = "";
std::string Configuration::s_colors[9]      = {};
bool Configuration::s_showAscii             = true;
bool Configuration::s_widthSupplied         = false;
bool Configuration::s_noNerdFonts           = false;
bool Configuration::s_distroSuppliedFromCli = false;

// Color Helper
static inline std::string GetColor(const std::string& name, const char* fallback) {
    // Lambda cause it's only called in this function anyways
    auto HexToAnsi = [](const std::string& hex, const char* fallback) -> std::string {
        const std::string h = hex[0] == '#' ? hex.substr(1) : hex;
        // Only works with 6 digit hex codes... for now
        if(h.size() != 6) return fallback;
        // Parse as 24 bit number
        unsigned int rgb = std::stoul(h, nullptr, 16);
        // Build ANSI 24-bit color from RGB
        return std::format("\033[38;2;{};{};{}m", (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    };
    if(name.empty()) return fallback;
    if(name[0] == '#') return HexToAnsi(name, fallback);
    static const std::unordered_map<std::string, std::string> map = {
        // Purple
        {"purple",         C::PURPLE},
        {"bold purple",    C::B_PURPLE},
        // Green
        {"green",          C::G},
        {"bright green",   C::BRIGHT_GREEN},
        {"bold green",     C::B_GREEN},
        // Yellow
        {"yellow",         C::YELLOW},
        {"bright yellow",  C::BRIGHT_YELLOW},
        {"bold yellow",    C::B_YELLOW},
        // Red
        {"red",            C::RED},
        {"bold red",       C::B_RED},
        // Blue
        {"blue",           C::BLUE},
        {"bright blue",    C::BRIGHT_BLUE},
        {"bold blue",      C::B_BLUE},
        {"bold bright blue", C::B_BRIGHT_BLUE},
    };
    auto it = map.find(name);
    return it != map.end() ? it->second : fallback;
}

std::string Configuration::GetConfigPath() {
    const std::map<const char*, std::filesystem::path> envConfigLocations = {{ "NITCHPP_CONFIG_FILE", "" }, { "XDG_CONFIG_HOME", "nitch++/config.toml" }, { "HOME", ".config/nitch++/config.toml" }};
    std::filesystem::path configPath;

    for(auto& [envVar, suffix] : envConfigLocations) {
        if(auto* val = std::getenv(envVar); val && *val) {
            configPath = std::filesystem::path(val) / suffix;
            break;
        }
    }
    if(configPath.empty())
        throw std::runtime_error("Could not determine config path: NITCHPP_CONFIG_FILE, XDG_CONFIG_HOME and HOME are all unset");

    // Ensure the directory exists no op if it does
    std::error_code ec;
    std::filesystem::create_directories(configPath.parent_path(), ec);
    if(ec) throw std::invalid_argument("Error: could not create config directory: " + configPath.parent_path().string());

    return configPath.string();
}

size_t Configuration::ParseConfigFile() {
    Icons icon;
    s_configFile = GetConfigPath();
    if(!std::filesystem::exists(s_configFile)) throw std::invalid_argument("Cannot find config file: '" + s_configFile + "'");

    toml::table tbl;
    try {
        tbl = toml::parse_file(s_configFile);
    } catch(const toml::parse_error& e) {
        throw std::invalid_argument("Failed to parse config file: '" + s_configFile + "'\nError: " + e.what());
    }

    toml::table* modsPtr = tbl["modules"].as_table();
    if(!modsPtr) throw std::invalid_argument("Missing [modules] section");
    toml::table& mods = *modsPtr;

    struct Spec { const char* name; const char* defIcon; const char* defColor; bool defShow; std::string* outIcon; bool* outShow; };
    std::array<Spec,9> specs = {{
        {"username", "", C::RED,           true,  &icon.s_iconUser,    &icon.s_showUsername},
        {"hostname", "", C::YELLOW,        true,  &icon.s_iconHname,   &icon.s_showHostname},
        {"distro",   "󰻀", C::G,             true,  &icon.s_iconDistro,  &icon.s_showDistro  },
        {"kernel",   "󰌢", C::BRIGHT_BLUE,   true,  &icon.s_iconKernel,  &icon.s_showKernel  },
        {"uptime",   "", C::BLUE,          true,  &icon.s_iconUptime,  &icon.s_showUptime  },
        {"shell",    "", C::PURPLE,        true,  &icon.s_iconShell,   &icon.s_showShell   },
        {"dewm",     "", C::G,             true,  &icon.s_iconDeWm,    &icon.s_showDeWm    },
        {"pkgs",     "󰏖", C::RED,           true,  &icon.s_iconPkgs,    &icon.s_showPkgs    },
        {"memory",   "󰍛", C::YELLOW,        true,  &icon.s_iconMemory,  &icon.s_showMemory  }
    }};

    auto distroFallback = [&]() -> std::string {
        auto it = Icons::s_distroIconMap.find(SystemInfo::s_distroID);
        return (it != Icons::s_distroIconMap.end()) ? it->second : "󰻀";
    };

    for(int i = 0; i < 9; i++) {
        Spec& s = specs[i];
        toml::array* arr = mods.at(s.name).as_array();
        if(!arr || arr->size() != 3) throw std::invalid_argument(std::string("modules.") + s.name + " must be [\"icon\", \"color\", true/false]");
        std::string raw = arr->at(0).value<std::string>().value_or("");
        *s.outIcon = (std::string(s.name) == "distro") ? (raw.empty() ? distroFallback() : raw) : (raw.empty() ? s.defIcon : raw);
        auto colorName = arr->at(1).value<std::string>().value_or("");
        s_colors[i] = GetColor(colorName, s.defColor);
        *s.outShow = arr->at(2).value<bool>().value_or(s.defShow);
    }

    toml::array* arr = mods.at("colors").as_array();
    if(!arr || arr->size() < 3) throw std::invalid_argument("modules.colors must be [icon, swatch, show]");

    icon.s_showNothing = std::all_of(specs.begin(), specs.end(), [&](auto &s){ return !*s.outShow; }) && !icon.s_showColors;

    Configuration::s_showAscii = tbl["general"]["show_ascii"].value_or(Configuration::s_showAscii);
    if(!s_widthSupplied) {
        Configuration::s_width = tbl["general"]["width"].value_or(Configuration::s_width);
        if(Configuration::s_width < 6) throw std::invalid_argument(C::B_RED + std::string("ERROR: ") + C::NC + "width in config must be >= 5");
    }

    if(Configuration::s_noNerdFonts) {
        for(Spec& s : specs) *s.outIcon = (std::string(s.name) == "colors" ? "~" : ">");
        icon.s_iconColorSwatches = "■";
    }

    auto asciiDistro = tbl["general"]["ascii_distro"].value_or(std::string{});
    if(Configuration::s_distroSuppliedFromCli || !asciiDistro.empty()) {
        const std::string& key = Configuration::s_distroSuppliedFromCli ? s_tmpDistro : asciiDistro;
        SystemInfo::s_logo = Logos::GetLogos(key);
        return 0;
    }
    auto asciiColorName = tbl["general"]["ascii_color"].value_or(std::string{"blue"});
    Configuration::s_asciiColor = GetColor(asciiColorName, C::B_BLUE);

    return 1;
}
