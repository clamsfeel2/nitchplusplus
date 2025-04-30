#include "configuration.h"
#include "system_info.h"
#include "colors.hpp"
#include "icons.h"
#include "logos.h"
#include <filesystem>
#include <toml++/toml.hpp>

std::string Configuration::s_configFile     = "";
std::string Configuration::s_tmpDistro      = "";
int Configuration::s_width                  = 6;
bool Configuration::s_showAscii             = true;
bool Configuration::s_widthSupplied         = false;
bool Configuration::s_noNerdFonts           = false;
bool Configuration::s_distroSuppliedFromCli = false;

std::string Configuration::GetConfigPath() {
    std::filesystem::path configPath;
    if(auto* env = std::getenv("NITCHPP_CONFIG_FILE"); env && *env) {
        configPath = env;
    } else if(auto* xdg = std::getenv("XDG_CONFIG_HOME"); xdg && *xdg) {
        configPath = std::filesystem::path(xdg) / "nitch++" / "config.toml";
    } else if(auto* home = std::getenv("HOME"); home && *home) {
        configPath = std::filesystem::path(home) / ".config" / "nitch++" / "config.toml";
    } else {
        throw std::runtime_error("Could not determine config path: " "NITCHPP_CONFIG_FILE, XDG_CONFIG_HOME and HOME are all unset");
    }
    // Ensure the directory exists no-op if it does
    std::error_code ec;
    std::filesystem::create_directories(configPath.parent_path(), ec);
    if(ec) throw std::invalid_argument("Error: could not create config directory: " + configPath.parent_path().string());
    return configPath.string();
}

size_t Configuration::ParseConfigFile() {
    Icons icon;
    s_configFile = GetConfigPath();
    if(!std::filesystem::exists(s_configFile)) return 1;

    toml::table tbl = toml::parse_file(s_configFile);
    toml::table* modsPtr = tbl["modules"].as_table();
    if(!modsPtr) throw std::invalid_argument("Missing [modules] section");
    toml::table& mods = *modsPtr;

    struct Spec {
        const char*   name;
        const char*   defIcon;
        bool          defShow;
        std::string*  outIcon;
        bool*         outShow;
    };

    std::vector<Spec> specs = {
        { "username", "", true,  &icon.s_iconUser,    &icon.s_showUsername },
        { "hostname", "", true,  &icon.s_iconHname,   &icon.s_showHostname },
        { "distro",   "󰻀", true,&icon.s_iconDistro,  &icon.s_showDistro     },
        { "kernel",   "󰌢", true,  &icon.s_iconKernel,  &icon.s_showKernel   },
        { "uptime",   "", true,  &icon.s_iconUptime,  &icon.s_showUptime   },
        { "shell",    "", true,  &icon.s_iconShell,   &icon.s_showShell    },
        { "dewm",     "", true, &icon.s_iconDeWm,    &icon.s_showDeWm      },
        { "pkgs",     "󰏖", true,  &icon.s_iconPkgs,    &icon.s_showPkgs      },
        { "memory",   "󰍛", true,  &icon.s_iconMemory,  &icon.s_showMemory   }
    };

    auto distroFallback = [&]() -> std::string {
        auto it = Icons::s_distroIconMap.find(SystemInfo::s_distroID);
        return (it != Icons::s_distroIconMap.end()) ? it->second : "󰻀";
    };

    for(Spec& s : specs) {
        if(toml::array* arr = mods.at(s.name).as_array()) {
            // Pull the raw first element (may be "")
            std::string raw = arr->size() > 0 ? arr->at(0).value<std::string>().value_or("") : "";

            if(std::string(s.name) == "distro") {
                // If distro use lambda to get icon if non-empty use what is in config
                *s.outIcon = raw.empty() ? distroFallback() : raw;
            } else {
                // If empty use fallback if not use what is in config
                *s.outIcon = raw.empty() ? s.defIcon : raw;
            }
            // Second element is always the show flag
            *s.outShow = arr->size() > 1 ? arr->at(1).value<bool>().value_or(s.defShow) : s.defShow;
        } else {
            // Missing or not an array
            *s.outIcon = s.defIcon;
            *s.outShow = s.defShow;
        }
    }

    if(toml::array* arr = mods.at("colors").as_array()) {
        if(arr->size() < 3) throw std::invalid_argument("modules.colors must be [icon,swatch,show]");
        icon.s_iconColors        = arr->at(0).value<std::string>().value_or("");
        icon.s_iconColorSwatches = arr->at(1).value<std::string>().value_or("");
        icon.s_showColors        = arr->at(2).value<bool>().value_or(true);
    }

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

    return 1;
}

