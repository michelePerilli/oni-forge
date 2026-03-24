#pragma once

#include "model/OniFile.hpp"
#include "model/ONCC.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Renders the editor view for ONCC (Character Class) files.
 *
 * This view handles the modification of character class attributes, such as
 * default animations, physics settings, and other metadata found in ONCC files.
 * It uses ProjectCatalogService to save changes back to disk.
 */
class ONCCView {
public:
    /**
     * @brief Constructs the ONCCView.
     *
     * @param vanilla Reference to VanillaCatalogService for listing related assets.
     * @param project Reference to ProjectCatalogService for saving updates.
     */
    explicit ONCCView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    /**
     * @brief Renders the header row for the currently selected file.
     *
     * This includes the file name input (for renaming) and the save button.
     *
     * @param file The currently selected ONCC file.
     * @param selectedIndex The index of the selected file in the project list.
     */
    void renderHeaderRow(OniFile<ONCC::Root>& file, int selectedIndex);

    /**
     * @brief Renders the main content of the ONCC editor.
     *
     * This method displays all editable fields and controls for the given ONCC file.
     *
     * @param file The currently selected ONCC file.
     * @param selectedIndex The index of the selected file.
     */
    void render(OniFile<ONCC::Root>& file, int selectedIndex);

private:
    const VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla data.
    ProjectCatalogService&       m_project; ///< Service for saving changes.

    /// Maps file index to its original path for tracking renames.
    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    /**
     * @brief Saves the current file, handling renames if the filename changed.
     *
     * @param file The file to save.
     * @param selectedIndex The index of the file in the project list.
     */
    void saveWithRename(const OniFile<ONCC::Root>& file, int selectedIndex);

    /**
     * @brief Helper to get a list of vanilla ONCV file names for dropdowns.
     *
     * @return A vector of ONCV filenames.
     */
    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;

    /**
     * @brief Helper to get a list of vanilla TRAC file names for dropdowns.
     *
     * @return A vector of TRAC filenames.
     */
    [[nodiscard]] std::vector<std::string> getVanillaTracNames() const;
};
