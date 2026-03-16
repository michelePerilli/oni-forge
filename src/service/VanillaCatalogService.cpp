#include "service/VanillaCatalogService.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include "component/logger/ILogger.hpp"

VanillaCatalogService::VanillaCatalogService(const OniRepositoryRegistry& repos, ILogger& logger)
    : m_repos(repos)
    , m_logger(logger) {}

void VanillaCatalogService::loadFromFolder(const std::filesystem::path& folderPath) {
    m_logger.info("[VanillaCatalogService] Loading vanilla files from: " + folderPath.string());
    loadOnccFiles(folderPath);
    loadOncvFiles(folderPath);
    loadTracFiles(folderPath);
    m_logger.info("[VanillaCatalogService] Loaded " +
                  std::to_string(m_onccFiles.size()) + " ONCC, " +
                  std::to_string(m_oncvFiles.size()) + " ONCV, " +
                  std::to_string(m_tracFiles.size()) + " TRAC files.");
}

const std::vector<OniFile<ONCC::Root>>& VanillaCatalogService::getOnccFiles() const {
    return m_onccFiles;
}

const std::vector<OniFile<ONCV::Root>>& VanillaCatalogService::getOncvFiles() const {
    return m_oncvFiles;
}

const std::vector<OniFile<TRAC::Root>>& VanillaCatalogService::getTracFiles() const {
    return m_tracFiles;
}

void VanillaCatalogService::loadOnccFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
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

void VanillaCatalogService::loadOncvFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
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

void VanillaCatalogService::loadTracFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
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
