#pragma once

#include <string>
#include <vector>

/**
 * @brief Represents a <TRACAnimation> element.
 */
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
struct TRAC {
    int id;
    std::string parentCollection;
    std::vector<TRACAnimation> animations;
};
