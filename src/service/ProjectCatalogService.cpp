#include "service/ProjectCatalogService.hpp"
#include "component/logger/ILogger.hpp"
#include "repository/OniRepositoryRegistry.hpp"

ProjectCatalogService::ProjectCatalogService(const OniRepositoryRegistry& repos,
                                             IOniCatalogService&          vanilla,
                                             const ILogger&               logger)
    : m_repos(repos)
      , m_vanilla(vanilla)
      , m_logger(logger) {
}

// ---------------------------------------------------------------------------
// Load
// ---------------------------------------------------------------------------

void ProjectCatalogService::loadFromFolder(const std::filesystem::path& folderPath) {
    m_logger.info("[ProjectCatalogService] Loading project files from: " + folderPath.string());
    loadOnccFiles(folderPath);
    loadOncvFiles(folderPath);
    loadTracFiles(folderPath);
    loadTramFiles(folderPath);
    m_logger.info("[ProjectCatalogService] Loaded " +
                  std::to_string(m_onccFiles.size()) + " ONCC, " +
                  std::to_string(m_oncvFiles.size()) + " ONCV, " +
                  std::to_string(m_tramFiles.size()) + " TRAM, " +
                  std::to_string(m_tracFiles.size()) + " TRAC files.");
}

void ProjectCatalogService::loadOnccFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::ONCCPrefix))
            continue;
        auto result = m_repos.oncc.load(path.string());
        if (!result) {
            m_logger.warning("[ProjectCatalogService] Failed to load ONCC: " + path.string());
            continue;
        }
        m_onccFiles.push_back(std::move(*result));
    }
}

void ProjectCatalogService::loadOncvFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::ONCVPrefix))
            continue;
        auto result = m_repos.oncv.load(path.string());
        if (!result) {
            m_logger.warning("[ProjectCatalogService] Failed to load ONCV: " + path.string());
            continue;
        }
        m_oncvFiles.push_back(std::move(*result));
    }
}

void ProjectCatalogService::loadTracFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::TRACPrefix))
            continue;
        auto result = m_repos.trac.load(path.string());
        if (!result) {
            m_logger.warning("[ProjectCatalogService] Failed to load TRAC: " + path.string());
            continue;
        }
        m_tracFiles.push_back(std::move(*result));
    }
}

void ProjectCatalogService::loadTramFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::TRAMPrefix))
            continue;
        auto result = m_repos.tram.load(path.string());
        if (!result) {
            m_logger.warning("[VanillaCatalogService] Failed to load TRAC: " + path.string());
            continue;
        }
        m_tramFiles.push_back(std::move(*result));
    }
}

// ---------------------------------------------------------------------------
// Save
// ---------------------------------------------------------------------------

void ProjectCatalogService::saveToFolder(const std::filesystem::path& folderPath) {
    m_logger.info("[ProjectCatalogService] Saving project to: " + folderPath.string());

    for (const auto& [path, data]: m_onccFiles) {
        if (OniFile out{folderPath / path.filename(), data}; !m_repos.oncc.save(out)) {
            m_logger.error("[ProjectCatalogService] error saving ONCC: " + path.filename().string());
        }
    }
    for (const auto& [path, data]: m_oncvFiles) {
        if (OniFile out{folderPath / path.filename(), data}; !m_repos.oncv.save(out)) {
            m_logger.error("[ProjectCatalogService] error saving ONCV: " + path.filename().string());
        }
    }
    for (const auto& [path, data]: m_tracFiles) {
        if (OniFile out{folderPath / path.filename(), data}; !m_repos.trac.save(out)) {
            m_logger.error("[ProjectCatalogService] error saving TRAC: " + path.filename().string());
        }
    }

    for (const auto& [path, data] : m_tramFiles) {
        if (OniFile out{folderPath / path.filename(), data}; !m_repos.tram.save(out))
            m_logger.error("[ProjectCatalogService] error saving TRAM: " + path.filename().string());
    }

    m_logger.info("[ProjectCatalogService] Project saved successfully.");
}

// ---------------------------------------------------------------------------
// Create from vanilla
// ---------------------------------------------------------------------------

bool ProjectCatalogService::createOnccFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getOnccFiles()) {
        if (file.path.stem().string() == name) {
            m_onccFiles.push_back(file);
            m_logger.info("[ProjectCatalogService] Added ONCC from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] ONCC not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createOncvFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getOncvFiles()) {
        if (file.path.stem().string() == name) {
            m_oncvFiles.push_back(file);
            m_logger.info("[ProjectCatalogService] Added ONCV from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] ONCV not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createTracFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getTracFiles()) {
        if (file.path.stem().string() == name) {
            m_tracFiles.push_back(file);
            m_logger.info("[ProjectCatalogService] Added TRAC from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] TRAC not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createTramFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getTramFiles()) {
        if (file.path.stem().string() == name) {
            m_tramFiles.push_back(file);
            m_logger.info("[ProjectCatalogService] Added TRAM from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] TRAM not found in vanilla: " + name);
    return false;
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------

const std::vector<OniFile<ONCC::Root>>& ProjectCatalogService::getOnccFiles() const {
    return m_onccFiles;
}

const std::vector<OniFile<ONCV::Root>>& ProjectCatalogService::getOncvFiles() const {
    return m_oncvFiles;
}

const std::vector<OniFile<TRAC::Root>>& ProjectCatalogService::getTracFiles() const {
    return m_tracFiles;
}

const std::vector<OniFile<TRAM::Root>>& ProjectCatalogService::getTramFiles() const {
    return m_tramFiles;
}
