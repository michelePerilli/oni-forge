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
 * @brief Renders the General tab for ONCV files.
 */
class ONCVView {
public:
    explicit ONCVView(const VanillaCatalogService& vanilla,
                      ProjectCatalogService&       project);

    void render(OniFile<ONCV::Root>& file, int selectedIndex);

private:
    const VanillaCatalogService& m_vanilla;
    ProjectCatalogService&       m_project;

    std::unordered_map<int, std::filesystem::path> m_originalPaths;

    void saveWithRename(const OniFile<ONCV::Root>& file, int selectedIndex);

    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;

    [[nodiscard]] std::vector<std::string> getVanillaOncvNamesStripped() const;
};
