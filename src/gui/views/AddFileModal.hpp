#pragma once

#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "component/logger/ILogger.hpp"

#include <string>
#include <unordered_set>

/**
 * @brief Renders the "Add file from vanilla" modal popup.
 */
class AddFileModal {
public:
    explicit AddFileModal(const VanillaCatalogService& vanilla,
                          ProjectCatalogService&       project,
                          const ILogger&               logger);

    /**
     * @brief Call each frame to show the modal if open.
     * Must be called outside of any child window.
     */
    void render();

    /**
     * @brief Opens the modal on the next render call.
     */
    void open();

    /**
     * @brief Called after new files are added to refresh snapshots if needed.
     */
    [[nodiscard]] bool wasJustConfirmed() const { return m_justConfirmed; }

private:
    const VanillaCatalogService& m_vanilla;
    ProjectCatalogService&       m_project;
    const ILogger&               m_logger;

    bool                            m_open          = false;
    bool                            m_justConfirmed = false;
    std::unordered_set<std::string> m_pendingFiles;

    [[nodiscard]] bool isInProject(const std::string& stem) const;
};
