#pragma once

#include "model/OniFile.hpp"
#include "model/TRAC.hpp"
#include "service/ProjectCatalogService.hpp"
#include "service/VanillaCatalogService.hpp"

#include <filesystem>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Renders the editor view for TRAC (Animation Collection) files.
 *
 * This view allows editing animation collections, which group multiple
 * TRAM (Animation Metadata) files together. It supports adding and removing
 * animations from the collection.
 */
class TRACView {
public:
    /**
     * @brief Constructs the TRACView.
     *
     * @param vanilla Reference to VanillaCatalogService for listing related assets.
     * @param project Reference to ProjectCatalogService for saving updates.
     */
    explicit TRACView(VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    /**
     * @brief Renders the header row for the currently selected file.
     *
     * Includes the file name input and save button.
     *
     * @param file The currently selected TRAC file.
     * @param selectedIndex The index of the selected file.
     */
    void renderHeaderRow(OniFile<TRAC::Root>& file, int selectedIndex);

    /**
     * @brief Renders the main content of the TRAC editor.
     *
     * Displays the list of animations in the collection and provides controls
     * to add new ones or remove existing ones.
     *
     * @param file The currently selected TRAC file.
     * @param selectedIndex The index of the selected file.
     */
    void render(OniFile<TRAC::Root>& file, int selectedIndex);

    /**
     * @brief Clears the selected animation index when the file changes.
     *
     * Ensures UI state is reset when switching between different TRAC files.
     */
    void onFileChanged() { m_selectedAnimIndices.clear(); }

private:
    VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla data.
    ProjectCatalogService&       m_project; ///< Service for saving changes.

    /// Maps file index to its original path to detect and handle renames.
    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    /// Tracks which animation items are currently selected in the UI list.
    std::unordered_set<int> m_selectedAnimIndices;

    /**
     * @brief Saves the current file, handling renames if necessary.
     *
     * @param file The file to save.
     * @param selectedIndex The index of the file in the project list.
     */
    void saveWithRename(OniFile<TRAC::Root>& file, int selectedIndex);
};
