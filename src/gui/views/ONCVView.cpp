#include "gui/views/ONCVView.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <imgui.h>

ONCVView::ONCVView(const VanillaCatalogService& vanilla,
                   ProjectCatalogService&       project)
    : m_vanilla(vanilla)
      , m_project(project) {
}

void ONCVView::render(OniFile<ONCV::Root>& file, const int selectedIndex) {
    if (!m_originalPaths.contains(selectedIndex))
        m_originalPaths[selectedIndex] = file.path;

    auto& [id, parentVariant, characterClass, characterClassHard] = file.data;

    constexpr float labelWidth = 140.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

    // Name + Save
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char              buf[256];
        const std::string stem = file.path.stem().string();
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##oncvname", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##oncv", {60, 0}))
        saveWithRename(file, selectedIndex);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Parent Variant
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Parent Variant:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        const auto         names   = getVanillaOncvNames();
        const std::string& current = parentVariant;
        if (ImGui::BeginCombo("##parentvariant", current.c_str())) {
            for (const auto& name: names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) parentVariant = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // Character Class
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Character Class:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        const auto         names   = getVanillaOncvNamesStripped();
        const std::string& current = characterClass;
        if (ImGui::BeginCombo("##characterclass", current.c_str())) {
            for (const auto& name: names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) characterClass = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // Character Class Hard
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Class Hard:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        const auto         names   = getVanillaOncvNamesStripped();
        const std::string& current = characterClassHard;
        if (ImGui::BeginCombo("##characterclasshard", current.c_str())) {
            for (const auto& name: names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) characterClassHard = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
}

void ONCVView::saveWithRename(const OniFile<ONCV::Root>& file, const int selectedIndex) {
    if (m_originalPaths.contains(selectedIndex)) {
        if (const auto& original = m_originalPaths[selectedIndex];
            original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalPaths[selectedIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}

std::vector<std::string> ONCVView::getVanillaOncvNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data]: m_vanilla.getOncvFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}

std::vector<std::string> ONCVView::getVanillaOncvNamesStripped() const {
    std::vector<std::string> names;
    for (const auto& [path, data]: m_vanilla.getOncvFiles()) {
        std::string stem = path.stem().string();
        if (stem.starts_with("ONCV"))
            stem = stem.substr(4);
        names.push_back(stem);
    }
    std::ranges::sort(names);
    return names;
}
