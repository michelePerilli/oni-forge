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
 * @brief Renders the General tab for TRAM files.
 */
class TRAMView {
public:
    explicit TRAMView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    void render(OniFile<TRAM::Root>& file, int selectedIndex);

private:
    const VanillaCatalogService& m_vanilla;
    ProjectCatalogService&       m_project;

    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    void saveWithRename(const OniFile<TRAM::Root>& file, int selectedIndex);

    static const std::vector<const char*> s_animTypes;
    static const std::vector<const char*> s_states;
    static const std::vector<const char*> s_varients;

    static void renderCombo(const char* label, std::string& value,
                            const std::vector<const char*>& items,
                            float labelWidth, float fieldWidth);
};