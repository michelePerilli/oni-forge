#include "service/ProjectCatalogService.hpp"
#include "component/logger/ILogger.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include <algorithm>

ProjectCatalogService::ProjectCatalogService(const OniRepositoryRegistry& repos,
                                             const IOniCatalogService&    vanilla,
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

    m_onccFiles.clear();
    m_oncvFiles.clear();
    m_tracFiles.clear();
    m_tramFiles.clear();

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
        auto& file = m_onccFiles.emplace_back(std::move(*result));
        file.status = FileStatus::Unmodified;
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
        auto& file = m_oncvFiles.emplace_back(std::move(*result));
        file.status = FileStatus::Unmodified;
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
        auto& file = m_tracFiles.emplace_back(std::move(*result));
        file.status = FileStatus::Unmodified;
    }
}

void ProjectCatalogService::loadTramFiles(const std::filesystem::path& folderPath) {
    for (const auto& entry: std::filesystem::directory_iterator(folderPath)) {
        const auto& path = entry.path();
        if (path.extension() != OniForge::xmlExtension || !path.stem().string().starts_with(OniForge::TRAMPrefix))
            continue;
        auto result = m_repos.tram.load(path.string());
        if (!result) {
            m_logger.warning("[ProjectCatalogService] Failed to load TRAM: " + path.string());
            continue;
        }
        auto& file = m_tramFiles.emplace_back(std::move(*result));
        file.status = FileStatus::Unmodified;
    }
}

// ---------------------------------------------------------------------------
// Save
// ---------------------------------------------------------------------------

void ProjectCatalogService::saveToFolder(const std::filesystem::path& folderPath) {
    m_logger.info("[ProjectCatalogService] Saving modified files to: " + folderPath.string());

    for (auto& file: m_onccFiles) {
        if (file.status != FileStatus::Unmodified && file.status != FileStatus::Deleted) saveFile(file);
    }
    for (auto& file: m_oncvFiles) {
        if (file.status != FileStatus::Unmodified && file.status != FileStatus::Deleted) saveFile(file);
    }
    for (auto& file: m_tracFiles) {
        if (file.status != FileStatus::Unmodified && file.status != FileStatus::Deleted) saveFile(file);
    }
    for (auto& file: m_tramFiles) {
        if (file.status != FileStatus::Unmodified && file.status != FileStatus::Deleted) saveFile(file);
    }

    m_logger.info("[ProjectCatalogService] Save completed.");
}

void ProjectCatalogService::saveFile(OniFile<ONCC::Root>& file) {
    if (m_repos.oncc.save(file)) {
        file.status = FileStatus::Unmodified;
        m_logger.info("[ProjectCatalogService] Saved ONCC: " + file.path.filename().string());
    } else {
        m_logger.error("[ProjectCatalogService] Error saving ONCC: " + file.path.filename().string());
    }
}

void ProjectCatalogService::saveFile(OniFile<ONCV::Root>& file) {
    if (m_repos.oncv.save(file)) {
        file.status = FileStatus::Unmodified;
        m_logger.info("[ProjectCatalogService] Saved ONCV: " + file.path.filename().string());
    } else {
        m_logger.error("[ProjectCatalogService] Error saving ONCV: " + file.path.filename().string());
    }
}

void ProjectCatalogService::saveFile(OniFile<TRAC::Root>& file) {
    if (m_repos.trac.save(file)) {
        file.status = FileStatus::Unmodified;
        m_logger.info("[ProjectCatalogService] Saved TRAC: " + file.path.filename().string());
    } else {
        m_logger.error("[ProjectCatalogService] Error saving TRAC: " + file.path.filename().string());
    }
}

void ProjectCatalogService::saveFile(OniFile<TRAM::Root>& file) {
    if (m_repos.tram.save(file)) {
        file.status = FileStatus::Unmodified;
        m_logger.info("[ProjectCatalogService] Saved TRAM: " + file.path.filename().string());
    } else {
        m_logger.error("[ProjectCatalogService] Error saving TRAM: " + file.path.filename().string());
    }
}

// ---------------------------------------------------------------------------
// Delete
// ---------------------------------------------------------------------------

void ProjectCatalogService::deleteFile(OniFile<ONCC::Root>& file) { file.status = FileStatus::Deleted; }
void ProjectCatalogService::deleteFile(OniFile<ONCV::Root>& file) { file.status = FileStatus::Deleted; }
void ProjectCatalogService::deleteFile(OniFile<TRAC::Root>& file) { file.status = FileStatus::Deleted; }
void ProjectCatalogService::deleteFile(OniFile<TRAM::Root>& file) { file.status = FileStatus::Deleted; }

// ---------------------------------------------------------------------------
// Create from vanilla
// ---------------------------------------------------------------------------

bool ProjectCatalogService::createOnccFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getOnccFiles()) {
        if (file.name == name) {
            auto copy = file;
            copy.status = FileStatus::Created;
            m_onccFiles.push_back(std::move(copy));
            m_logger.info("[ProjectCatalogService] Added ONCC from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] ONCC not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createOncvFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getOncvFiles()) {
        if (file.name == name) {
            auto copy = file;
            copy.status = FileStatus::Created;
            m_oncvFiles.push_back(std::move(copy));
            m_logger.info("[ProjectCatalogService] Added ONCV from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] ONCV not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createTracFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getTracFiles()) {
        if (file.name == name) {
            auto copy = file;
            copy.status = FileStatus::Created;
            m_tracFiles.push_back(std::move(copy));
            m_logger.info("[ProjectCatalogService] Added TRAC from vanilla: " + name);
            return true;
        }
    }
    m_logger.warning("[ProjectCatalogService] TRAC not found in vanilla: " + name);
    return false;
}

bool ProjectCatalogService::createTramFromVanilla(const std::string& name) {
    for (const auto& file: m_vanilla.getTramFiles()) {
        if (file.name == name) {
            auto copy = file;
            copy.status = FileStatus::Created;
            m_tramFiles.push_back(std::move(copy));
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
