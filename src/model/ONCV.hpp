#pragma once

#include <string>

/**
 * @brief Represents the full content of an ONCV file.
 *
 * Maps directly to the <ONCV> tag and its children.
 * Field names match the corresponding XML tag names.
 */
struct ONCV {
    int id;
    std::string parentVariant;
    std::string characterClass;
    std::string characterClassHard;
};
