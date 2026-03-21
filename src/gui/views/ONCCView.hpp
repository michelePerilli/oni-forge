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
 * @brief Renders the General tab for ONCC files.
 */
class ONCCView {
public:
    explicit ONCCView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    void render(OniFile<ONCC::Root>& file, int selectedIndex);

private:
    const VanillaCatalogService& m_vanilla;
    ProjectCatalogService&       m_project;

    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    void saveWithRename(const OniFile<ONCC::Root>& file, int selectedIndex);

    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;

    [[nodiscard]] std::vector<std::string> getVanillaTracNames() const;
};
