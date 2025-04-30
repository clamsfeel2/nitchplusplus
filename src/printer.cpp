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
    auto repeat       =[](std::string_view s,int n) -> std::string { std::string o; for(; n--; ) o+=s; return o; };
    const int  width            = Configuration::s_width;
    const std::string dashLine = repeat("─", width + 5);
    bool anyPrinted             = false;

    if(Configuration::s_showAscii) std::cout << C::B_BLUE << si.s_logo << C::NC << "\n";
    if(icon.s_showNothing) return;

    std::vector<Row> rows = {
        { icon.s_showUsername, icon.s_iconUser,    "user",   si.user,         C::RED         },
        { icon.s_showHostname, icon.s_iconHname,   "hname",  si.hostname,     C::YELLOW      },
        { icon.s_showDistro,   icon.s_iconDistro,  "distro", si.distro,       C::G           },
        { icon.s_showKernel,   icon.s_iconKernel,  "kernel", si.kernel,       C::BRIGHT_BLUE },
        { icon.s_showUptime,   icon.s_iconUptime,  "uptime", si.uptime,       C::BLUE        },
        { icon.s_showShell,    icon.s_iconShell,   "shell",  si.shell,        C::PURPLE      },
        { icon.s_showDeWm,     icon.s_iconDeWm,    "de/wm",  si.de,           C::G           },
        { icon.s_showPkgs,      icon.s_iconPkgs,    "pkgs",   si.packageCount, C::RED         },
        { icon.s_showMemory,   icon.s_iconMemory,  "memory", si.memory,       C::YELLOW      }
    };

    // Strip stray newlines
    for(Row& r : rows)
        if(!r.value.empty() && r.value.back() == '\n') r.value.pop_back();

    // Helper for every row
    auto printRow = [&](const Row& r) -> bool {
        if(!r.show) return false;
        auto padded = r.label + std::string(width - r.label.size(), ' ');
        std::cout << "  │ " << r.color << r.icon << " " << C::NC << " " << padded << " │ " << r.color << r.value << C::NC << "\n";
        return true;
    };

    std::cout << "  ╭" << dashLine << "╮\n";
    for(Row& r : rows)
        anyPrinted |= printRow(r);

    // colors
    if(icon.s_showColors) {
        if(anyPrinted) std::cout << "  ├" << dashLine << "┤\n";
        std::string label = "colors";
        std::cout << "  │ " << icon.s_iconColors << "  " << label << std::string(width - label.size(), ' ') << " │ " << PrintColors() << "\n";
    }
    std::cout << "  ╰" << dashLine << "╯\n";
}

std::string Printer::PrintColors() {
    Icons icon;
    std::ostringstream oss;
    constexpr const char* cols[] = { C::NC, C::RED, C::YELLOW, C::G, C::BRIGHT_BLUE, C::BLUE, C::PURPLE };
    for(const char* col : cols) oss << col << icon.s_iconColorSwatches << ' ';
    oss << C::NC;
    return oss.str();
}
