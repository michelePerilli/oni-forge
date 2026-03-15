#pragma once

#include <string>

namespace ONCV {
    /**
     * @brief Represents the full content of an ONCV file.
     *
     * Maps directly to the <ONCV> tag and its children.
     * Field names match the corresponding XML tag names.
     */
    struct Root {
        int id;
        std::string parentVariant;
        std::string characterClass;
        std::string characterClassHard;
    };
}
