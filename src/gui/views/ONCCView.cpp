/**
 * @file ONCCView.cpp
 * @brief Implementation of the ONCC (Character Class) editor view.
 */

#include "gui/views/ONCCView.hpp"
#include "component/validation/OniValidator.hpp"
#include <imgui.h>
#include <algorithm>
#include <filesystem>
#include <cstring>

/**
 * @brief Constructs an ONCC editor view with references to catalog services.
 * @param vanilla Reference to the vanilla catalog service for read-only data.
 * @param project Reference to the project catalog service for modifiable data.
 */
ONCCView::ONCCView(VanillaCatalogService& vanilla,
                   ProjectCatalogService&       project)
    : m_vanilla(vanilla)
      , m_project(project) {
}

/**
 * @brief Renders the editor's top bar (file renaming and saving).
 * @param file The ONCC file being edited.
 * @param selectedIndex The index of the file in the open files list.
 */
void ONCCView::renderHeaderRow(OniFile<ONCC::Root>& file, const int selectedIndex) {
    constexpr float labelWidth = 120.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;
    ImGui::SeparatorText("Character Class (ONCC)");

    // Filename input and Save button
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("File Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char              buf[256];
        const std::string& stem = file.name;
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##filename", buf, sizeof(buf))) {
            file.name = buf;
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##oncc", {60, 0})) {
        saveWithRename(file, selectedIndex);
    }
    ImGui::Spacing();
}

/**
 * @brief Main entry point for rendering the ONCC editor's tabbed interface.
 * @details Handles validation triggering and tab navigation.
 * @param file The ONCC file being edited.
 * @param selectedIndex The index of the file in the open files list.
 */
void ONCCView::render(OniFile<ONCC::Root>& file, const int selectedIndex) {
    if (!m_originalPaths.contains(selectedIndex))
        m_originalPaths[selectedIndex] = file.path;

    // Trigger validation pass if needed
    if (m_needValidation) {
        m_validationResult = OniForge::Validation::checkOnccImpacts(file, m_project, m_vanilla);
        m_needValidation   = false;
    }

    if (ImGui::BeginTabBar("ONCCTabs")) {
        if (ImGui::BeginTabItem("General")) {
            renderGeneralTab(file);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Impacts")) {
            renderImpactsTab(file);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

/**
 * @brief Renders the "General" tab for core ONCC properties.
 * @details Includes variant selection, animation collection, stats, and flags.
 * @param file The ONCC file being edited.
 */
void ONCCView::renderGeneralTab(OniFile<ONCC::Root>& file) {
    ONCC::ONCC&     oncc       = file.data.oncc;
    constexpr float labelWidth = 120.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 20.0f;

    ImGui::Spacing();

    // Variant Selection
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Variant (ONCV):");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        std::string& current = oncc.variant;
        static bool  wasOpen = false;
        if (ImGui::BeginCombo("##variant", current.c_str())) {
            static char filter[128] = {};
            if (!wasOpen) { filter[0] = '\0'; wasOpen = true; }

            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##variantfilter", filter, sizeof(filter));
            ImGui::Separator();

            const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 6.0f;
            ImGui::BeginChild("##variantlist", {0, listHeight}, false);

            ImGui::SeparatorText("Project Catalog");
            for (const auto& [path, name, data]: m_project.getOncvFiles()) {
                if (filter[0] != '\0' && name.find(filter) == std::string::npos) continue;
                if (name == current) continue;
                if (ImGui::Selectable(name.c_str(), false)) {
                    current   = name;
                    filter[0] = '\0';
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::SeparatorText("Vanilla Catalog");
            for (const auto& [path, name, data]: m_vanilla.getOncvFiles()) {
                if (filter[0] != '\0' && name.find(filter) == std::string::npos) continue;
                if (name == current) continue;
                if (ImGui::Selectable(name.c_str(), false)) {
                    current   = name;
                    filter[0] = '\0';
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndChild();
            ImGui::EndCombo();
        } else { wasOpen = false; }
    }

    // Animation Collection Selection
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Anims (TRAC):");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        std::string& current = oncc.animations;
        static bool  wasOpen = false;
        if (ImGui::BeginCombo("##animations", current.c_str())) {
            static char filter[128] = {};
            if (!wasOpen) { filter[0] = '\0'; wasOpen = true; }

            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##animationsfilter", filter, sizeof(filter));
            ImGui::Separator();

            const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 6.0f;
            ImGui::BeginChild("##animationslist", {0, listHeight}, false);

            ImGui::SeparatorText("Project Catalog");
            for (const auto& [path, name, data]: m_project.getTracFiles()) {
                if (filter[0] != '\0' && name.find(filter) == std::string::npos) continue;
                if (name == current) continue;
                if (ImGui::Selectable(name.c_str(), false)) {
                    current          = name;
                    filter[0]        = '\0';
                    m_needValidation = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::SeparatorText("Vanilla Catalog");
            for (const auto& [path, name, data]: m_vanilla.getTracFiles()) {
                if (filter[0] != '\0' && name.find(filter) == std::string::npos) continue;
                if (name == current) continue;
                if (ImGui::Selectable(name.c_str(), false)) {
                    current          = name;
                    filter[0]        = '\0';
                    m_needValidation = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndChild();
            ImGui::EndCombo();
        } else { wasOpen = false; }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Stats
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Health:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f); {
        char buf[64];
        strncpy(buf, oncc.health.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##health", buf, sizeof(buf))) oncc.health = buf;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Weapon Hand:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f); {
        char buf[64];
        strncpy(buf, oncc.weaponHand.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##weaponhand", buf, sizeof(buf))) oncc.weaponHand = buf;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Flags
    bool hasDaodan = (oncc.hasDaodanPowers == "1");
    bool hasSuper  = (oncc.hasSupershield == "1");
    bool cantTouch = (oncc.cantTouchThis == "1");

    if (ImGui::Checkbox("Has Daodan Powers", &hasDaodan)) oncc.hasDaodanPowers = hasDaodan ? "1" : "0";
    if (ImGui::Checkbox("Has Supershield", &hasSuper)) oncc.hasSupershield = hasSuper ? "1" : "0";
    if (ImGui::Checkbox("Can't Touch This", &cantTouch)) oncc.cantTouchThis = cantTouch ? "1" : "0";
}

/**
 * @brief Renders the "Impacts" tab for managing the character's particle registration.
 * @details Allows viewing, adding, and removing impact registrations. 
 *          Integrates with the validation system to show missing impacts.
 * @param file The ONCC file being edited.
 */
void ONCCView::renderImpactsTab(OniFile<ONCC::Root>& file) {
    auto& impacts = file.data.onia.impacts;

    if (m_selectedImpacts.size() != impacts.size()) {
        m_selectedImpacts.resize(impacts.size(), false);
    }

    ImGui::Spacing();
    if (ImGui::Button("+ Add New Impact")) {
        impacts.push_back({"", "", ""});
        m_selectedImpacts.push_back(false);
        m_needValidation = true;
    }
    ImGui::SameLine();

    const bool anySelected = std::ranges::any_of(m_selectedImpacts, [](bool b) { return b; });
    if (!anySelected) ImGui::BeginDisabled();
    if (ImGui::Button("Delete Selected")) {
        for (int i = static_cast<int>(impacts.size()) - 1; i >= 0; i--) {
            if (m_selectedImpacts[i]) {
                impacts.erase(impacts.begin() + i);
                m_selectedImpacts.erase(m_selectedImpacts.begin() + i);
            }
        }
        m_needValidation = true;
    }
    if (!anySelected) ImGui::EndDisabled();

    // Impacts Table
    if (ImGui::BeginTable("ImpactsTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("##sel", ImGuiTableColumnFlags_WidthFixed, 25.0f);
        ImGui::TableSetupColumn("Impact Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Modifier");
        ImGui::TableHeadersRow();

        for (int i = 0; i < static_cast<int>(impacts.size()); i++) {
            ImGui::TableNextRow();
            ImGui::PushID(i);

            ImGui::TableNextColumn();
            bool isSelected = m_selectedImpacts[i];
            if (ImGui::Checkbox("##sel", &isSelected)) {
                m_selectedImpacts[i] = isSelected;
            }

            auto renderCell = [&](const char* id, std::string& val, bool triggerValid = false) {
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                char buf[128];
                snprintf(buf, sizeof(buf), "%s", val.c_str());
                if (ImGui::InputText(id, buf, sizeof(buf))) {
                    val = buf;
                    if (triggerValid) m_needValidation = true;
                }
            };

            renderCell("##n", impacts[i].name, true);
            renderCell("##t", impacts[i].type);
            renderCell("##m", impacts[i].modifier);

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    // Validation Warnings Section
    if (!m_validationResult.unregisteredImpacts.empty()) {
        ImGui::Spacing();
        ImGui::SeparatorText("Validation: Missing Impact Registrations");
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        ImGui::TextWrapped("The following impacts are used by animations but are not registered in the ONCP list:");
        
        for (const auto& impact: m_validationResult.unregisteredImpacts) {
            ImGui::BulletText("%s", impact.c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton(("Add Registration##" + impact).c_str())) {
                impacts.push_back({impact, "Swt_Super_Punch", "Heavy"});
                m_selectedImpacts.push_back(false);
                m_needValidation = true;
            }
        }
        ImGui::PopStyleColor();
    }
}

/**
 * @brief Handles file saving while tracking and removing old files if a rename occurred.
 * @param file The ONCC file to save.
 * @param selectedIndex The index of the file in the open files list.
 */
void ONCCView::saveWithRename(const OniFile<ONCC::Root>& file, const int selectedIndex) {
    if (m_originalPaths.contains(selectedIndex)) {
        if (const auto& original = m_originalPaths[selectedIndex];
            original != file.path && std::filesystem::exists(original)) {
            std::filesystem::remove(original);
        }
        m_originalPaths[selectedIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}
