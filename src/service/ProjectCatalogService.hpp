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
 * Loads and saves mod files. Depends on VanillaCatalogService
 * as a read-only reference for creating new entries from vanilla.
 */
class ProjectCatalogService : public IOniCatalogService {
public:
    ProjectCatalogService(const OniRepositoryRegistry& repos,
                          const IOniCatalogService& vanilla,
                          const ILogger& logger);

    /**
     * @brief Loads all supported files from the given project folder.
     * @param folderPath Path to the mod project folder.
     */
    void loadFromFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Saves all project files to the given output folder.
     * @param folderPath Path to the output folder.
     */
    void saveToFolder(const std::filesystem::path& folderPath);

    /**
     * @brief Creates an editable copy of a vanilla ONCC file in the project.
     * @param name The filename stem to look up in the vanilla catalog (e.g. "ONCCbarabus").
     * @return True if the file was found in vanilla and added to the project.
     */
    [[nodiscard]] bool createOnccFromVanilla(const std::string& name);

    [[nodiscard]] bool createOncvFromVanilla(const std::string& name);

    [[nodiscard]] bool createTracFromVanilla(const std::string& name);

    [[nodiscard]] const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const override;

    [[nodiscard]] const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const override;

    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

private:
    const OniRepositoryRegistry& m_repos;
    const IOniCatalogService& m_vanilla;
    const ILogger& m_logger;

    std::vector<OniFile<ONCC::Root>> m_onccFiles;
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;
    std::vector<OniFile<TRAC::Root>> m_tracFiles;

    void loadOnccFiles(const std::filesystem::path& folderPath);

    void loadOncvFiles(const std::filesystem::path& folderPath);

    void loadTracFiles(const std::filesystem::path& folderPath);
};
