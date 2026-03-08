#include "printer.h"
#include "configuration.h"
#include "system_info.h"
#include "icons.h"
#include "colors.hpp"
#include <array>
#include <string>
#include <sstream>
#include <print>

void Printer::Print() {
    struct Row { bool show; std::string icon; std::string label; std::string value; std::string color; };
    SystemInfo    si;
    Configuration cfg;
    Icons         icon;

    si.InitializeDistroID();
    si.Initialize(cfg.ParseConfigFile() == 1);

    auto repeatUTF8       = [](std::string_view s,int n) -> std::string { std::string o; for(; n--; ) o+=s; return o; };
    const int  width                = Configuration::s_width;
    const std::string dashLine      = repeatUTF8("─", width + 5);
    bool anyPrinted                 = false;

    if(Configuration::s_showAscii) std::println("{}{}{}", Configuration::s_asciiColor, si.s_logo, C::NC);
    if(icon.s_showNothing) return;

    std::array<Row, 9> rows = {{
    { icon.s_showUsername, icon.s_iconUser,   "user",   si.user,         Configuration::s_colors[0] },
    { icon.s_showHostname, icon.s_iconHname,  "hname",  si.hostname,     Configuration::s_colors[1] },
    { icon.s_showDistro,   icon.s_iconDistro, "distro", si.distro,       Configuration::s_colors[2] },
    { icon.s_showKernel,   icon.s_iconKernel, "kernel", si.kernel,       Configuration::s_colors[3] },
    { icon.s_showUptime,   icon.s_iconUptime, "uptime", si.uptime,       Configuration::s_colors[4] },
    { icon.s_showShell,    icon.s_iconShell,  "shell",  si.shell,        Configuration::s_colors[5] },
    { icon.s_showDeWm,     icon.s_iconDeWm,   "de/wm",  si.de,           Configuration::s_colors[6] },
    { icon.s_showPkgs,     icon.s_iconPkgs,   "pkgs",   si.packageCount, Configuration::s_colors[7] },
    { icon.s_showMemory,   icon.s_iconMemory, "memory", si.memory,       Configuration::s_colors[8] }
}};

    for(Row& row : rows) if(!row.value.empty() && row.value.back() == '\n') row.value.pop_back();

    auto printRow = [&](const Row& row) -> bool {
        if(!row.show) return false;
        auto padded = row.label + std::string(width - row.label.size(), ' ');
        std::println("  │ {}{} {} {} │ {}{}{}", row.color, row.icon, C::NC, padded, row.color, row.value, C::NC);
        return true;
    };

    std::println("  ╭{}╮", dashLine);

    for(Row& row : rows) anyPrinted |= printRow(row);

    if(icon.s_showColors) {
        if(anyPrinted) std::println("  ├{}┤", dashLine);
        std::string label = "colors";
        std::println("  │ {}  {}{} │ {}", icon.s_iconColors, label, std::string(width - label.size(), ' '), PrintColors());
    }
    std::println("  ╰{}╯", dashLine);
}

std::string Printer::PrintColors() {
    Icons icon;
    std::ostringstream oss;
    // Use module colors from config or default
    const std::string defaults[] = { C::NC, C::RED, C::YELLOW, C::G, C::BRIGHT_BLUE, C::BLUE, C::PURPLE };
    for(int i = 0; i < 7; i++) {
        const std::string& col = (i > 0 && !Configuration::s_colors[i-1].empty()) ? Configuration::s_colors[i-1] : defaults[i];
        oss << col << icon.s_iconColorSwatches << ' ';
    }
    oss << C::NC;
    return oss.str();
}
