#pragma once

#include "gui/OniForgeTheme.hpp"
#include <string>

namespace OniForge {

/**
 * @brief Application configuration and user settings.
 */
struct Config {
    // --- Paths ---
    std::string vanillaPath  = R"(D:\Dev\java\oniforge\vanilla\xml)";
    std::string projectPath  = R"(D:\Dev\mods\oni\TCTFagent)";
    std::string oniSplitPath = R"(D:\Dev\mods\oni\.tools\OniSplit.exe)";
    std::string oniGamePath  = R"(D:\Program Files (x86)\Oni\AE)";
    std::string tempOniPath  = R"(D:\Dev\mods\oni\TCTFagent\_build)";

    // --- Preferences ---
    Theme theme     = Theme::Neutral;
    int   fontSize  = 15;
};

} // namespace OniForge
