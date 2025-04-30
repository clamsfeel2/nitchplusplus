#include "icons.h"

std::unordered_map<std::string, std::string> Icons::s_distroIconMap = {
    { "ubuntu",      ""    },
    { "arch",        "󰣇"    },
    { "debian",      ""    },
    { "fedora",      ""    },
    { "mint",        "󰣭"    },
    { "zorin",       ""    },
    { "popos",       ""    },
    { "manjaro",     ""    },
    { "opensuse",    ""    },
    { "centos",      ""    },
   { "gentoo",      ""    },
   { "endeavouros", ""    },
   { "artix",       ""    },
   { "void",        ""    },
   { "macos",       ""    },
   { "nixos",       ""    }
};
std::string	Icons::s_iconUser("");
std::string Icons::s_iconHname("");
std::string Icons::s_iconKernel("󰌢");
std::string Icons::s_iconUptime("");
std::string Icons::s_iconDistro("󰻀");
std::string Icons::s_iconShell("");
std::string Icons::s_iconDeWm("");
std::string Icons::s_iconPkgs("󰏖");
std::string Icons::s_iconMemory("󰍛");
std::string Icons::s_iconColors("");
std::string Icons::s_iconColorSwatches("");

bool Icons::s_showNothing(false);
bool Icons::s_showUsername(true);
bool Icons::s_showHostname(true);
bool Icons::s_showDistro(true);
bool Icons::s_showKernel(true);
bool Icons::s_showUptime(true);
bool Icons::s_showShell(true);
bool Icons::s_showDeWm(true);
bool Icons::s_showPkgs(true);
bool Icons::s_showMemory(true);
bool Icons::s_showColors(true);
