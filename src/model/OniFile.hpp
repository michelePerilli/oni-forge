#pragma once
#include <filesystem>
#include <string_view>

/**
 * @brief Wraps any Oni data struct with its filesystem context.
 *
 * T is the data struct (ONCV, TRAC, TRMA, etc.)
 * The path is the source of truth for the filename — used as primary key.
 *
 * @tparam T The type of the data struct.
 */
template<typename T>
struct OniFile {
    /// @brief The source path of the file on disk.
    std::filesystem::path path;
    /// @brief The deserialized data struct.
    T data;
};

/**
 * @brief Project-wide constants for OniForge.
 */
namespace OniForge {
    /// @brief The standard XML file extension.
    constexpr std::string_view xmlExtension  = ".xml";
    /// @brief Prefix for Character Class files.
    constexpr std::string_view ONCCPrefix    = "ONCC";
    /// @brief Prefix for Character Variant files.
    constexpr std::string_view ONCVPrefix    = "ONCV";
    /// @brief Prefix for Track files.
    constexpr std::string_view TRACPrefix    = "TRAC";
    /// @brief Prefix for Track Material files.
    constexpr std::string_view TRAMPrefix    = "TRAM";
}
