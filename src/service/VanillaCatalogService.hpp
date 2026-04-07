#pragma once

#include <filesystem>
#include <vector>
#include <mutex>

#include "service/IOniCatalogService.hpp"

// Forward declarations
struct OniRepositoryRegistry;
class ILogger;

/**
 * @brief Read-only catalog of vanilla Oni files.
 *
 * This service is responsible for loading and maintaining a read-only index of
 * standard Oni game files (ONCC, ONCV, TRAC, TRAM) from a specified vanilla
 * installation directory. It acts as the immutable source of truth for base game data.
 */
class VanillaCatalogService final : public IOniCatalogService {
public:
    /**
     * @brief Constructs a new Vanilla Catalog Service.
     *
     * @param repos Reference to the repository registry for loading files.
     * @param logger Reference to the logger for diagnostic output.
     */
    VanillaCatalogService(const OniRepositoryRegistry& repos, const ILogger& logger);

    /**
     * @brief Loads all supported files from the given vanilla folder.
     *
     * Scans the directory and its subdirectories for supported file types and
     * populates the internal catalogs.
     *
     * @param folderPath Path to the vanilla data folder.
     */
    void loadFromFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Retrieves the list of loaded vanilla ONCC (Character Class) files.
     * @return A constant reference to the vector of ONCC files.
     */
    [[nodiscard]] const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const override;

    /**
     * @brief Retrieves the list of loaded vanilla ONCV (Character Variant) files.
     * @return A constant reference to the vector of ONCV files.
     */
    [[nodiscard]] const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const override;

    /**
     * @brief Retrieves the list of loaded vanilla TRAC (Animation Collection) files.
     * @return A constant reference to the vector of TRAC files.
     */
    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

    /**
     * @brief Retrieves the list of loaded vanilla TRAM (Animation Mapping) files.
     * @return A constant reference to the vector of TRAM files.
     */
    [[nodiscard]] const std::vector<OniFile<TRAM::Root>>& getTramFiles() const override;

    /**
     * @brief Gets the mutex for thread-safe access to catalog collections.
     * @return Reference to the recursive mutex.
     */
    [[nodiscard]] std::recursive_mutex& getMutex() const override { return m_mutex; }

private:
    const OniRepositoryRegistry& m_repos;   ///< Registry of repositories for file operations.
    const ILogger&               m_logger;  ///< Logger instance.

    mutable std::recursive_mutex m_mutex; ///< Mutex to protect access to file vectors.

    std::vector<OniFile<ONCC::Root>> m_onccFiles;
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;
    std::vector<OniFile<TRAC::Root>> m_tracFiles;
    std::vector<OniFile<TRAM::Root>> m_tramFiles;

    /**
     * @brief Helper to load ONCC files from the directory.
     * @param folderPath The root directory to search in.
     */
    void loadOnccFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load ONCV files from the directory.
     * @param folderPath The root directory to search in.
     */
    void loadOncvFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAC files from the directory.
     * @param folderPath The root directory to search in.
     */
    void loadTracFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAM files from the directory.
     * @param folderPath The root directory to search in.
     */
    void loadTramFiles(const std::filesystem::path& folderPath);
};
