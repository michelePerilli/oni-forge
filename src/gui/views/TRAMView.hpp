#pragma once

#include "model/OniFile.hpp"
#include "model/TRAM.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Renders the editor view for TRAM (Animation Metadata) files.
 *
 * This view handles the modification of individual animation properties, such as
 * playback speed, looping behavior, and event triggers. It works in conjunction
 * with TRAC files, which are collections of TRAMs.
 */
class TRAMView {
public:
    /**
     * @brief Constructs the TRAMView.
     *
     * @param vanilla Reference to VanillaCatalogService for listing related assets.
     * @param project Reference to ProjectCatalogService for saving updates.
     */
    explicit TRAMView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    /**
     * @brief Renders the main content of the TRAM editor.
     *
     * Displays all editable animation parameters and event timelines.
     *
     * @param file The currently selected TRAM file.
     * @param selectedIndex The index of the selected file.
     */
    void render(OniFile<TRAM::Root>& file, int selectedIndex);

    /**
     * @brief Renders the header row for the currently selected file.
     *
     * Includes the file name input and save button.
     *
     * @param file The currently selected TRAM file.
     * @param selectedIndex The index of the selected file.
     */
    void renderHeaderRow(OniFile<TRAM::Root>& file, int selectedIndex);

    /**
     * @brief Clears the selected animation name when the file changes.
     *
     * Ensures UI state is reset when switching between TRAM files.
     */
    void onFileChanged() { m_selectedAnimName.clear(); }

private:
    const VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla data.
    ProjectCatalogService&       m_project; ///< Service for saving changes.

    /// Maps file index to its original path to detect and handle renames.
    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    /// Stores the name of the currently selected animation variant (if applicable).
    std::string m_selectedAnimName;

    /**
     * @brief Saves the current file, handling renames if necessary.
     *
     * @param file The file to save.
     * @param selectedIndex The index of the file in the project list.
     */
    void saveWithRename(const OniFile<TRAM::Root>& file, int selectedIndex);

    /// List of available animation variants for dropdowns.
    static const std::vector<const char*> s_varients;

    /**
     * @brief Helper to render a custom combo box for animation variants.
     *
     * @param label The label for the combo box.
     * @param value The currently selected value.
     * @param items The list of available items.
     * @param labelWidth The width of the label column.
     * @param fieldWidth The width of the input field column.
     */
    static void renderCombo(const char*                     label, std::string& value,
                            const std::vector<const char*>& items,
                            float                           labelWidth, float fieldWidth);
};
