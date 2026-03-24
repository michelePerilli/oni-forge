#pragma once

#include "model/OniFile.hpp"
#include "model/ONCV.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Renders the editor view for ONCV (Character Variant) files.
 *
 * This view handles the modification of character variant attributes, including
 * skin, costume, and model references. It provides a visual editor for
 * ONCV data structures.
 */
class ONCVView {
public:
    /**
     * @brief Constructs the ONCVView.
     *
     * @param vanilla Reference to VanillaCatalogService for listing related assets.
     * @param project Reference to ProjectCatalogService for saving updates.
     */
    explicit ONCVView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    /**
     * @brief Renders the header row for the currently selected file.
     *
     * Typically includes the file name input and save button.
     *
     * @param file The currently selected ONCV file.
     * @param selectedIndex The index of the selected file.
     */
    void renderHeaderRow(OniFile<ONCV::Root>& file, int selectedIndex);

    /**
     * @brief Renders the main content of the ONCV editor.
     *
     * This method displays all editable fields and dropdowns for selecting
     * related models and textures.
     *
     * @param file The currently selected ONCV file.
     * @param selectedIndex The index of the selected file.
     */
    void render(OniFile<ONCV::Root>& file, int selectedIndex);

private:
    const VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla data.
    ProjectCatalogService&       m_project; ///< Service for saving changes.

    /// Maps file index to its original path to detect and handle renames.
    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    /**
     * @brief Saves the current file, handling renames if necessary.
     *
     * @param file The file to save.
     * @param selectedIndex The index of the file in the project list.
     */
    void saveWithRename(const OniFile<ONCV::Root>& file, int selectedIndex);

    /**
     * @brief Helper to get vanilla ONCV names for suggestions.
     *
     * @return A vector of ONCV filenames.
     */
    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;

    /**
     * @brief Helper to get vanilla ONCV names stripped of extensions.
     *
     * Useful for displaying clean names in the UI.
     *
     * @return A vector of base ONCV names.
     */
    [[nodiscard]] std::vector<std::string> getVanillaOncvNamesStripped() const;
};
