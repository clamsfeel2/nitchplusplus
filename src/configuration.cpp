#include "configuration.h"
#include "system_info.h"
#include "colors.hpp"
#include "icons.h"
#include "logos.h"
#include <filesystem>
#include <toml++/toml.hpp>

bool Configuration::showAscii = true;
bool Configuration::widthSupplied = false;
bool Configuration::noNerdFonts = false;
std::string Configuration::configFile = "";
std::string Configuration::tmpDistro = "";
bool Configuration::distroSuppliedFromCli;
int Configuration::width = 6;

std::string Configuration::GetConfigPath() {
    // Use either $NITCHPP_CONFIG_FILE or ~/.config/nitch++/config.toml
    const char* env = std::getenv("NITCHPP_CONFIG_FILE");
    std::filesystem::path configPath = env ? std::filesystem::path(env) : std::filesystem::path(std::getenv("HOME")) / ".config" / "nitch++" / "config.toml";

    // Ensure the parent directory exists
    std::error_code ec;
    std::filesystem::create_directories(configPath.parent_path(), ec);
    if(ec)
        throw std::invalid_argument("Error: Could not create config directory: " + configPath.parent_path().string());

    return configPath.string();
}

// FIXME when pkgs is false in config this throws a seg fault. WTF?!?!
size_t Configuration::ParseConfigFile() {
    Icons icon;
    configFile = GetConfigPath();
    if(!std::filesystem::exists(configFile)) return 1;

    toml::table tbl = toml::parse_file(configFile);
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
        { "username", "", true,  &icon.iconUser,    &icon.showUsername },
        { "hostname", "", true,  &icon.iconHname,   &icon.showHostname },
        { "distro",   "󰻀", true,&icon.iconDistro,  &icon.showDistro     },
        { "kernel",   "󰌢", true,  &icon.iconKernel,  &icon.showKernel   },
        { "uptime",   "", true,  &icon.iconUptime,  &icon.showUptime   },
        { "shell",    "", true,  &icon.iconShell,   &icon.showShell    },
        { "dewm",     "", true, &icon.iconDeWm,    &icon.showDeWm      },
        { "pkgs",     "󰏖", true,  &icon.iconPkgs,    &icon.showPkg      },
        { "memory",   "󰍛", true,  &icon.iconMemory,  &icon.showMemory   }
    };

    auto distroFallback = [&]() -> std::string {
        auto it = Icons::distroIconMap.find(SystemInfo::distroID);
        return (it != Icons::distroIconMap.end()) ? it->second : "󰻀";
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
        if(arr->size() < 3)
            throw std::invalid_argument("modules.colors must be [icon,swatch,show]");
        icon.iconColors        = arr->at(0).value<std::string>().value_or("");
        icon.iconColorSwatches = arr->at(1).value<std::string>().value_or("");
        icon.showColors        = arr->at(2).value<bool>().value_or(true);
    }

    icon.showNothing = std::all_of(specs.begin(), specs.end(), [&](auto &s){ return !*s.outShow; }) && !icon.showColors;

    Configuration::showAscii = tbl["general"]["show_ascii"].value_or(Configuration::showAscii);
    if(!widthSupplied) {
        Configuration::width = tbl["general"]["width"].value_or(Configuration::width);
        if(Configuration::width < 6) throw std::invalid_argument(C::B_RED + std::string("ERROR: ") + C::NC + "width in config must be >= 5");
    }

    if(Configuration::noNerdFonts) {
        for(Spec& s : specs) *s.outIcon = (std::string(s.name) == "colors" ? "~" : ">");
        icon.iconColorSwatches = "■";
    }

    auto asciiDistro = tbl["general"]["ascii_distro"].value_or(std::string{});
    if(Configuration::distroSuppliedFromCli || !asciiDistro.empty()) {
        const std::string& key = Configuration::distroSuppliedFromCli ? tmpDistro : asciiDistro;
        SystemInfo::logo = Logos::GetLogos(key);
        return 0;
    }

    return 1;
}

