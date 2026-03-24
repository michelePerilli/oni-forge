#pragma once

#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "component/logger/ILogger.hpp"

#include <string>
#include <unordered_set>

/**
 * @brief Renders the "Add file from vanilla" modal popup.
 *
 * This modal allows users to browse vanilla game files (ONCC, ONCV, TRAC, TRAM)
 * and add them to the current project. It handles listing available vanilla
 * assets and creating new copies in the project directory.
 */
class AddFileModal {
public:
    /**
     * @brief Constructs the AddFileModal.
     *
     * @param vanilla Reference to the VanillaCatalogService for listing base game assets.
     * @param project Reference to the ProjectCatalogService for adding files to the project.
     * @param logger Reference to the ILogger for logging operations.
     */
    explicit AddFileModal(const VanillaCatalogService& vanilla,
                          ProjectCatalogService&       project,
                          const ILogger&               logger);

    /**
     * @brief Renders the modal window if it is currently open.
     *
     * This method must be called every frame outside of any other ImGui window
     * begin/end block. It handles the internal logic for displaying the file
     * list, filtering, and processing the "Add" action.
     */
    void render();

    /**
     * @brief Opens the modal on the next render call.
     *
     * Sets the internal state to open the popup.
     */
    void open();

    /**
     * @brief Checks if files were added in the last frame.
     *
     * Use this to trigger a refresh of the file lists in the main application
     * after the user has successfully added new files.
     *
     * @return True if files were just added and confirmed, false otherwise.
     */
    [[nodiscard]] bool wasJustConfirmed() const { return m_justConfirmed; }

private:
    const VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla game data.
    ProjectCatalogService&       m_project; ///< Service for managing project files.
    const ILogger&               m_logger;  ///< Logger instance.

    bool                            m_open          = false; ///< Current open state of the modal.
    bool                            m_justConfirmed = false; ///< Flag indicating if an action was confirmed this frame.
    std::unordered_set<std::string> m_pendingFiles;          ///< Set of selected files pending addition.

    /**
     * @brief Checks if a file with the given stem already exists in the project.
     *
     * @param stem The filename stem (without extension) to check.
     * @return True if the file exists in the project, false otherwise.
     */
    [[nodiscard]] bool isInProject(const std::string& stem) const;
};
