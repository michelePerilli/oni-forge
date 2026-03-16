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
 * Loads all supported file types from a vanilla folder.
 * Acts as the source of truth — nothing here is ever modified.
 */
class VanillaCatalogService final : public IOniCatalogService {
public:
    VanillaCatalogService(const OniRepositoryRegistry& repos, const ILogger& logger);

    /**
     * @brief Loads all supported files from the given vanilla folder.
     * @param folderPath Path to the vanilla data folder.
     */
    void loadFromFolder(const std::filesystem::path& folderPath);

    [[nodiscard]] const std::vector<OniFile<ONCC::Root>>& getOnccFiles() const override;

    [[nodiscard]] const std::vector<OniFile<ONCV::Root>>& getOncvFiles() const override;

    [[nodiscard]] const std::vector<OniFile<TRAC::Root>>& getTracFiles() const override;

private:
    const OniRepositoryRegistry& m_repos;
    const ILogger& m_logger;

    std::vector<OniFile<ONCC::Root>> m_onccFiles;
    std::vector<OniFile<ONCV::Root>> m_oncvFiles;
    std::vector<OniFile<TRAC::Root>> m_tracFiles;

    void loadOnccFiles(const std::filesystem::path& folderPath);

    void loadOncvFiles(const std::filesystem::path& folderPath);

    void loadTracFiles(const std::filesystem::path& folderPath);
};
