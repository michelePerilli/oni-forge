#pragma once

#include "model/OniFile.hpp"
#include "model/TRAM.hpp"
#include "service/ProjectCatalogService.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

/**
 * @brief Renders the General tab for TRAM files.
 */
class TRAMView {
public:
    explicit TRAMView(ProjectCatalogService& project);

    void render(OniFile<TRAM::Root>& file, int selectedIndex);

private:
    ProjectCatalogService& m_project;

    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    void saveWithRename(const OniFile<TRAM::Root>& file, int selectedIndex);

    // Predefined value lists
    static const std::vector<const char*> s_animTypes;
    static const std::vector<const char*> s_states;
    static const std::vector<const char*> s_varients;

    // Helper for combo with predefined values
    static void renderCombo(const char* label, std::string& value,
                            const std::vector<const char*>& items,
                            float labelWidth, float fieldWidth);
};
