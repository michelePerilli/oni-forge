#pragma once

#include <string>
#include <vector>

namespace TRAC {
    struct TRACAnimation {
        int weight;
        std::string animation;
    };

    /**
     * @brief Represents the full content of a TRAC file.
     *
     * Maps directly to the <TRAC> tag and its children.
     * Field names match the corresponding XML tag names.
     */
    struct Root {
        int id;
        std::string parentCollection;
        std::vector<TRACAnimation> animations;
    };
} // namespace TRAC
