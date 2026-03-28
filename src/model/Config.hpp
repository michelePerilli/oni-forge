#pragma once

#include "gui/OniForgeTheme.hpp"
#include <string>

namespace OniForge {

/**
 * @brief Application configuration and user settings.
 */
struct Config {
    // --- Paths ---
    /// @brief Path to the vanilla XML files.
    std::string vanillaPath  = R"(D:\Dev\java\oniforge\vanilla\xml)";
    /// @brief Path to the current project directory.
    std::string projectPath  = R"(D:\Dev\mods\oni\TCTFagent)";
    /// @brief Path to the OniSplit executable.
    std::string oniSplitPath = R"(D:\Dev\mods\oni\.tools\OniSplit.exe)";
    /// @brief Path to the Oni game directory.
    std::string oniGamePath  = R"(D:\Program Files (x86)\Oni\AE)";
    /// @brief Path for temporary Oni files during build.
    std::string tempOniPath  = R"(D:\Dev\mods\oni\TCTFagent\_build)";

    // --- Preferences ---
    /// @brief The selected UI theme.
    Theme theme     = Theme::Neutral;
    /// @brief The font size for the UI.
    int   fontSize  = 15;
};

} // namespace OniForge
