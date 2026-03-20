#pragma once

#include <imgui.h>
#include <string>

enum class Theme {
    Dark,
    Light,
    Classic,
    Neutral  // custom mid-tone theme
};

/**
 * @brief Applies the given theme to the current ImGui context.
 */
void applyTheme(Theme theme);

/**
 * @brief Returns the display name of a theme.
 */
const char* themeName(Theme theme);
