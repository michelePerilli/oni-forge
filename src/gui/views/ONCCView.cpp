#include "gui/views/ONCCView.hpp"

#include <imgui.h>
#include <algorithm>
#include <filesystem>
#include <cstring>

ONCCView::ONCCView(const VanillaCatalogService& vanilla,
                       ProjectCatalogService&       project)
    : m_vanilla(vanilla)
    , m_project(project) {}

void ONCCView::render(OniFile<ONCC::Root>& file, const int selectedIndex) {
    // Snapshot path on first encounter of this index
    if (!m_originalPaths.contains(selectedIndex))
        m_originalPaths[selectedIndex] = file.path;

    ONCC::ONCC& oncc = file.data.oncc;

    constexpr float labelWidth = 120.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

    // Name + Save
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        char buf[256];
        const std::string stem = file.path.stem().string();
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##name", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##oncc", {60, 0}))
        saveWithRename(file, selectedIndex);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // Variant
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Variant:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNames();
        const std::string& current = oncc.variant;
        if (ImGui::BeginCombo("##variant", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncc.variant = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // Animations
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Animations:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaTracNames();
        const std::string& current = oncc.animations;
        if (ImGui::BeginCombo("##animations", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncc.animations = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // Health
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Health:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.health.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##health", buf, sizeof(buf))) oncc.health = buf;
    }

    // Weapon Hand
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Weapon Hand:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.weaponHand.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##weaponhand", buf, sizeof(buf))) oncc.weaponHand = buf;
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // Flags
    bool hasDaodan = (oncc.hasDaodanPowers == "1");
    bool hasSuper  = (oncc.hasSupershield  == "1");
    bool cantTouch = (oncc.cantTouchThis   == "1");

    if (ImGui::Checkbox("Has Daodan Powers", &hasDaodan)) oncc.hasDaodanPowers = hasDaodan ? "1" : "0";
    if (ImGui::Checkbox("Has Supershield",   &hasSuper))  oncc.hasSupershield  = hasSuper  ? "1" : "0";
    if (ImGui::Checkbox("Can't Touch This",  &cantTouch)) oncc.cantTouchThis   = cantTouch ? "1" : "0";
}

void ONCCView::saveWithRename(const OniFile<ONCC::Root>& file, const int selectedIndex) {
    if (m_originalPaths.contains(selectedIndex)) {
        if (const auto& original = m_originalPaths[selectedIndex]; original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalPaths[selectedIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}

std::vector<std::string> ONCCView::getVanillaOncvNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getOncvFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}

std::vector<std::string> ONCCView::getVanillaTracNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getTracFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}
