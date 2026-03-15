#pragma once
#include <filesystem>

/**
 * @brief Wraps any Oni data struct with its filesystem context.
 *
 * T is the data struct (ONCV, TRAC, TRMA, etc.)
 * The path is the source of truth for the filename — used as primary key.
 */
template<typename T>
struct OniFile {
    std::filesystem::path path;
    T data;
};