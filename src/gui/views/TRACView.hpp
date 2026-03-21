#pragma once

#include "model/OniFile.hpp"
#include "model/TRAC.hpp"
#include "service/ProjectCatalogService.hpp"

#include <filesystem>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Renders the General tab for TRAC files.
 */
class TRACView {
public:
    explicit TRACView(ProjectCatalogService& project);

    void render(OniFile<TRAC::Root>& file, int selectedIndex);

    void onFileChanged() { m_selectedAnimIndices.clear(); }

private:
    ProjectCatalogService& m_project;

    std::unordered_map<int, std::filesystem::path> m_originalPaths;
    std::unordered_set<int>                        m_selectedAnimIndices;

    void saveWithRename(const OniFile<TRAC::Root>& file, int selectedIndex);
};
