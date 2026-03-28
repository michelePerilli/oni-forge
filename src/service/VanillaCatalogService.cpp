#include "service/VanillaCatalogService.hpp"

#include <bits/ranges_algo.h>

#include "component/logger/ILogger.hpp"
#include "repository/OniRepositoryRegistry.hpp"

/**
 * @brief Constructs the vanilla catalog service.
 */
VanillaCatalogService::VanillaCatalogService(const OniRepositoryRegistry& repos, const ILogger& logger)
    : m_repos(repos)
      , m_logger(logger) {
}

/**
 * @brief Orchestrates the loading of all vanilla assets from the file system.
 */
void VanillaCatalogService::loadFromFolder(const std::filesystem::path& folderPath) {
    m_logger.info("[VanillaCatalogService] Loading vanilla files from: " + folderPath.string());
    
    m_onccFiles.clear();
    m_oncvFiles.clear();
    m_tracFiles.clear();
    m_tramFiles.clear();
    
    loadOnccFiles(folderPath);
    loadOncvFiles(folderPath);
    loadTracFiles(folderPath);
    loadTramFiles(folderPath);


    m_logger.info("[VanillaCatalogService] Loaded " +
                  std::to_string(m_onccFiles.size()) + " ONCC, " +
                  std::to_string(m_oncvFiles.size()) + " ONCV, " +
                  std::to_string(m_tramFiles.size()) + " TRAM, " +
                  std::to_string(m_tracFiles.size()) + " TRAC files.");
}

/**
 * @brief Public accessor for ONCC files.
 */
const std::vector<OniFile<ONCC::Root>>& VanillaCatalogService::getOnccFiles() const {
    return m_onccFiles;
}


/**
 * @brief Public accessor for ONCV files.
 */
const std::vector<OniFile<ONCV::Root>>& VanillaCatalogService::getOncvFiles() const {
    return m_oncvFiles;
}


/**
 * @brief Public accessor for TRAC files.
 */
const std::vector<OniFile<TRAC::Root>>& VanillaCatalogService::getTracFiles() const {
    return m_tracFiles;
}


/**
 * @brief Public accessor for TRAM files.
 */
const std::vector<OniFile<TRAM::Root>>& VanillaCatalogService::getTramFiles() const {
    return m_tramFiles;
}


/**
 * @brief Scans and loads all .xml files starting with 'ONCC'.
 */
void VanillaCatalogService::loadOnccFiles(const std::filesystem::path& folderPath) {
    if (!std::filesystem::exists(folderPath)) return;
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::ONCCPrefix))
            continue;
        auto result = m_repos.oncc.load(path.string());
        if (!result) {
            m_logger.warning("[VanillaCatalogService] Failed to load ONCC: " + path.string());
            continue;
        }
        m_onccFiles.push_back(std::move(*result));
    }
}

/**
 * @brief Scans and loads all .xml files starting with 'ONCV'.
 */
void VanillaCatalogService::loadOncvFiles(const std::filesystem::path& folderPath) {
    if (!std::filesystem::exists(folderPath)) return;
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::ONCVPrefix))
            continue;
        auto result = m_repos.oncv.load(path.string());
        if (!result) {
            m_logger.warning("[VanillaCatalogService] Failed to load ONCV: " + path.string());
            continue;
        }
        m_oncvFiles.push_back(std::move(*result));
    }
}

/**
 * @brief Scans and loads all .xml files starting with 'TRAC'.
 */
void VanillaCatalogService::loadTracFiles(const std::filesystem::path& folderPath) {
    if (!std::filesystem::exists(folderPath)) return;
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::TRACPrefix))
            continue;
        auto result = m_repos.trac.load(path.string());
        if (!result) {
            m_logger.warning("[VanillaCatalogService] Failed to load TRAC: " + path.string());
            continue;
        }
        m_tracFiles.push_back(std::move(*result));
    }
}

/**
 * @brief Scans and loads all .xml files starting with 'TRAM'.
 */
void VanillaCatalogService::loadTramFiles(const std::filesystem::path& folderPath) {
    if (!std::filesystem::exists(folderPath)) return;
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::TRAMPrefix))
            continue;
        auto result = m_repos.tram.load(path.string());
        if (!result) {
            m_logger.warning("[VanillaCatalogService] Failed to load TRAM: " + path.string());
            continue;
        }
        m_tramFiles.push_back(std::move(*result));
    }
}
