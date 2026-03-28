#pragma once

#include "model/OniFile.hpp"
#include "model/ONCC.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "component/validation/OniValidator.hpp"

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
    explicit ONCCView(VanillaCatalogService& vanilla,
                      ProjectCatalogService& project);

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
    VanillaCatalogService& m_vanilla; ///< Service for accessing vanilla data.
    ProjectCatalogService& m_project; ///< Service for saving changes.

    /// Maps file index to its original path for tracking renames.
    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    /// Stores the results of the last validation run for impact effects.
    OniForge::Validation::OnccImpactValidationResult m_validationResult;

    /// Flag to trigger a fresh validation pass on the next render.
    bool m_needValidation = true;

    /// Tracking selected rows in the impacts table for bulk operations.
    std::vector<bool> m_selectedImpacts;

    /**
     * @brief Saves the current file, handling renames if the filename changed.
     *
     * @param file The file to save.
     * @param selectedIndex The index of the file in the project list.
     */
    void saveWithRename(const OniFile<ONCC::Root>& file, int selectedIndex);

    /**
     * @brief Renders the "General" tab containing core ONCC properties.
     * 
     * @param file The ONCC file to edit.
     */
    void renderGeneralTab(OniFile<ONCC::Root>& file);

    /**
     * @brief Renders the "Impacts" tab for managing particle effect registrations.
     * 
     * @param file The ONCC file to edit.
     */
    void renderImpactsTab(OniFile<ONCC::Root>& file);

};
