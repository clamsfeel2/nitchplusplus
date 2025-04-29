#include "printer.h"
#include "configuration.h"
#include "system_info.h"
#include "icons.h"
#include "colors.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

struct Row {
    bool        show;
    std::string icon;
    std::string label;
    std::string value;
    std::string color;
};

void Printer::Print() {
    SystemInfo    si;
    Configuration cfg;
    Icons         icon;

    si.InitializeDistroID();
    si.Initialize(cfg.ParseConfigFile() == 1);

    // Helper to repeat a UTF-8 string
    auto repeat       =[](std::string_view s,int n) -> std::string{ std::string o; for(; n--; ) o+=s; return o; };
    const int  width            = Configuration::width;
    const std::string dashLine  = repeat("──", width);
    bool anyPrinted             = false;

    if(Configuration::showAscii) std::cout << C::B_BLUE << si.logo << C::NC << "\n";
    if(icon.showNothing) return;

    std::vector<Row> rows = {
        { icon.showUsername, icon.iconUser,    "user",   si.user,         C::RED         },
        { icon.showHostname, icon.iconHname,   "hname",  si.hostname,     C::YELLOW      },
        { icon.showDistro,   icon.iconDistro,  "distro", si.distro,       C::G           },
        { icon.showKernel,   icon.iconKernel,  "kernel", si.kernel,       C::BRIGHT_BLUE },
        { icon.showUptime,   icon.iconUptime,  "uptime", si.uptime,       C::BLUE        },
        { icon.showShell,    icon.iconShell,   "shell",  si.shell,        C::PURPLE      },
        { icon.showDeWm,     icon.iconDeWm,    "de/wm",  si.de,           C::G           },
        { icon.showPkg,      icon.iconPkgs,    "pkgs",   si.packageCount, C::RED },
        { icon.showMemory,   icon.iconMemory,  "memory", si.memory,       C::YELLOW      }
    };

    // Strip stray newlines
    for(auto &r : rows)
        if(!r.value.empty() && r.value.back() == '\n') r.value.pop_back();

    // Helper for every row
    auto printRow = [&](const Row& r) -> bool {
        if(!r.show) return false;
        auto padded = r.label + std::string(width - r.label.size(), ' ');
        std::cout << "  │ " << r.color << r.icon << " " << C::NC << " " << padded << "  │ " << r.color << r.value << C::NC << "\n";
        return true;
    };

    std::cout << "  ╭" << dashLine << "╮\n";
    for(auto& r : rows)
        anyPrinted |= printRow(r);

    // colors
    if(icon.showColors) {
        if(anyPrinted) std::cout << "  ├" << dashLine << "┤\n";
        std::string label = "colors";
        std::cout << "  │ " << icon.iconColors << "  " << label << std::string(width - label.size(), ' ') << "  │ " << PrintColors() << "\n";
    }
    std::cout << "  ╰" << dashLine << "╯\n";
}

std::string Printer::PrintColors() {
    Icons icon;
    std::ostringstream oss;
    constexpr const char* cols[] = { C::NC, C::RED, C::YELLOW, C::G, C::BRIGHT_BLUE, C::BLUE, C::PURPLE };
    for(auto col : cols) oss << col << icon.iconColorSwatches << ' ';
    oss << C::NC;
    return oss.str();
}
