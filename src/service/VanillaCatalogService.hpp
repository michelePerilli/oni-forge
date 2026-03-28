#pragma once

#include <filesystem>
#include <vector>

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
     * @brief Retrieves a list of sorted vanilla ONCV file names for dropdowns.
     *
     * This method uses a cache to avoid repeated sorting and path manipulations.
     *
     * @return A reference to the vector of sorted ONCV filenames.
     */
    std::vector<std::string>& getVanillaOncvNames();

    /**
     * @brief Retrieves the list of loaded vanilla TRAC (Animation Collection) files.
     * @return A constant reference to the vector of TRAC files.
     */
    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

    /**
     * @brief Retrieves a list of sorted vanilla TRAC file names for dropdowns.
     *
     * This method uses a cache to avoid repeated sorting and path manipulations.
     *
     * @return A vector of sorted TRAC filenames.
     */
    std::vector<std::string>& getVanillaTracNames();

    /**
     * @brief Retrieves the list of loaded vanilla TRAM (Animation Mapping) files.
     * @return A constant reference to the vector of TRAM files.
     */
    [[nodiscard]] const std::vector<OniFile<TRAM::Root>>& getTramFiles() const override;

private:
    const OniRepositoryRegistry& m_repos;   ///< Registry of repositories for file operations.
    const ILogger&               m_logger;  ///< Logger instance.

    std::vector<OniFile<ONCC::Root>> m_onccFiles;   ///< Storage for loaded ONCC files.
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;   ///< Storage for loaded ONCV files.
    
    std::vector<std::string> m_oncvNamesCache;      ///< Cached sorted names of ONCV files.
    bool m_oncvNamesCacheDirty = true;              ///< Flag to refresh ONCV cache.

    std::vector<OniFile<TRAC::Root>> m_tracFiles;   ///< Storage for loaded TRAC files.
    
    std::vector<std::string> m_tracNamesCache;      ///< Cached sorted names of TRAC files.
    bool m_tracNamesCacheDirty = true;              ///< Flag to refresh TRAC cache.

    std::vector<OniFile<TRAM::Root>> m_tramFiles;   ///< Storage for loaded TRAM files.

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
