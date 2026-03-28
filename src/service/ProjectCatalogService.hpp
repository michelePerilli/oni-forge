#pragma once

#include <filesystem>
#include <vector>

#include "service/IOniCatalogService.hpp"

// Forward declarations
struct OniRepositoryRegistry;
class ILogger;

/**
 * @brief Editable catalog representing the current mod project.
 *
 * This service manages the loading, saving, and modification of mod files within a specific project folder.
 * It also facilitates the creation of new mod files by copying data from the read-only vanilla catalog.
 *
 * Responsibilities:
 * - Loading project-specific ONCC, ONCV, TRAC, and TRAM files.
 * - Saving modifications back to the project folder.
 * - Cloning vanilla assets into the project for editing.
 */
class ProjectCatalogService final : public IOniCatalogService {
public:
    /**
     * @brief Constructs the ProjectCatalogService with necessary dependencies.
     *
     * @param repos Reference to the repository registry for file I/O operations.
     * @param vanilla Reference to the read-only vanilla catalog for sourcing base game data.
     * @param logger Reference to the logger for diagnostic output.
     */
    ProjectCatalogService(const OniRepositoryRegistry& repos,
                          const IOniCatalogService&    vanilla,
                          const ILogger&               logger);

    /**
     * @brief Loads all supported mod files from the specified project directory.
     *
     * Scans the directory for ONCC, ONCV, TRAC, and TRAM files and populates the internal collections.
     *
     * @param folderPath The root path of the mod project.
     */
    void loadFromFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Saves all loaded project files back to the specified output directory.
     *
     * Iterates through all internal file collections and serializes them to XML in the target folder.
     *
     * @param folderPath The destination path for saving files.
     */
    void saveToFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Creates an editable copy of a vanilla ONCC file in the project.
     *
     * Looks up the file in the vanilla catalog by name. If found, a deep copy is added to the project's
     * collection, allowing for modification without affecting the source.
     *
     * @param name The filename stem (e.g., "ONCCbarabus") to search for in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     * @return false If the file was not found in the vanilla catalog.
     */
    [[nodiscard]] bool createOnccFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla ONCV file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     * @return false If the file was not found.
     */
    [[nodiscard]] bool createOncvFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla TRAC file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     * @return false If the file was not found.
     */
    [[nodiscard]] bool createTracFromVanilla(const std::string& name);

    /**
     * @brief Creates an editable copy of a vanilla TRAM file in the project.
     *
     * @param name The filename stem to look up in the vanilla catalog.
     * @return true If the file was found in vanilla and successfully added to the project.
     * @return false If the file was not found.
     */
    [[nodiscard]] bool createTramFromVanilla(const std::string& name);

    /**
     * @brief Retrieves the collection of loaded ONCC (Character Class) files.
     * @return A constant reference to the vector of ONCC files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const override;

    /**
     * @brief Retrieves the collection of loaded ONCV (Character Variant) files.
     * @return A constant reference to the vector of ONCV files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const override;

    /**
     * @brief Retrieves the collection of loaded TRAC (Animation Collection) files.
     * @return A constant reference to the vector of TRAC files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

    /**
     * @brief Retrieves the collection of loaded TRAM (Animation Metadata) files.
     * @return A constant reference to the vector of TRAM files in the project.
     */
    [[nodiscard]] const std::vector<OniFile<TRAM::Root>>& getTramFiles() const override;

private:
    const OniRepositoryRegistry& m_repos;   ///< Registry containing repositories for file I/O.
    const IOniCatalogService&    m_vanilla; ///< Reference to the base game data service.
    const ILogger&               m_logger;  ///< Logger for error reporting and info messages.

    std::vector<OniFile<ONCC::Root>> m_onccFiles;
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;
    std::vector<OniFile<TRAC::Root>> m_tracFiles;
    std::vector<OniFile<TRAM::Root>> m_tramFiles;
    /**
     * @brief Helper to load ONCC files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadOnccFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load ONCV files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadOncvFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAC files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadTracFiles(const std::filesystem::path& folderPath);

    /**
     * @brief Helper to load TRAM files from the directory.
     * @param folderPath The directory to scan.
     */
    void loadTramFiles(const std::filesystem::path& folderPath);
};
