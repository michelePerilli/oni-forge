#include "gui/views/TRAMView.hpp"
#include "gui/OniUI.hpp"

#include <cstring>
#include <filesystem>
#include <imgui.h>
#include <pugixml.hpp>

#include "service/ProjectCatalogService.hpp"
#include "service/VanillaCatalogService.hpp"

// ---------------------------------------------------------------------------
// Predefined value lists
// ---------------------------------------------------------------------------

const std::vector<const char*> TRAMView::s_varients = {
    "Combat",
    "LeftPistol",
    "LeftRifle",
    "Panic",
    "RightPistol",
    "RightRifle",
    "Sprint",
};

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

TRAMView::TRAMView(const VanillaCatalogService& vanilla, ProjectCatalogService& project)
    : m_vanilla(vanilla)
      , m_project(project) {
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

void TRAMView::renderHeaderRow(OniFile<TRAM::Root>& file, const int selectedIndex) {
    constexpr float labelWidth = 140.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;
    
    std::string title = "Animation Metadata (TRAM)";

    OniUI::PushFileStatusColor(file.status);
    ImGui::SeparatorText(title.c_str());
    OniUI::PopFileStatusColor();

    // --- Name + Save ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("File Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char              buf[256];
        const std::string& stem = file.name;
        std::snprintf(buf, sizeof(buf), "%s", stem.c_str());
        if (ImGui::InputText("##tramname", buf, sizeof(buf))) {
            file.name = buf;
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##tram", {60, 0}))
        saveWithRename(file, selectedIndex);

    ImGui::Spacing();
}

void TRAMView::render(OniFile<TRAM::Root>& file, const int selectedIndex) {
    if (!m_originalPaths.contains(selectedIndex))
        m_originalPaths[selectedIndex] = file.path;

    TRAM::Root& tram = file.data;

    constexpr float labelWidth = 140.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

    // --- Import path (read-only info) ---
    if (tram.importPath) {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("DAE Import:");
        ImGui::SameLine(labelWidth);
        ImGui::TextDisabled("%s", tram.importPath->c_str());
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }

    // --- Lookup ---
    if (ImGui::CollapsingHeader("Lookup", ImGuiTreeNodeFlags_DefaultOpen))
        if (ImGui::BeginTable("##lookup", 5, ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("label1", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("value1", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableSetupColumn("spacer", ImGuiTableColumnFlags_WidthFixed, 16.0f);
            ImGui::TableSetupColumn("label2", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableSetupColumn("value2", ImGuiTableColumnFlags_WidthStretch, 2.0f);

            // Row 1 — Type / Aiming Type
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Type:");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##type", tram.lookup.type.c_str())) {
                for (const char* item: TRAM::animTypes) {
                    const bool selected = (tram.lookup.type == item);
                    if (ImGui::Selectable(item, selected)) {
                        tram.lookup.type = item;
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::TableSetColumnIndex(3);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Aiming Type:");
            ImGui::TableSetColumnIndex(4);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##aimingtype", tram.lookup.aimingType.c_str())) {
                for (const char* item: TRAM::animTypes) {
                    const bool selected = (tram.lookup.aimingType == item);
                    if (ImGui::Selectable(item, selected)) {
                        tram.lookup.aimingType = item;
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Row 2 — From State / To State
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("From State:");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##fromstate", tram.lookup.fromState.c_str())) {
                for (const char* item: TRAM::animStates) {
                    const bool selected = (tram.lookup.fromState == item);
                    if (ImGui::Selectable(item, selected)) {
                        tram.lookup.fromState = item;
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::TableSetColumnIndex(3);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("To State:");
            ImGui::TableSetColumnIndex(4);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##tostate", tram.lookup.toState.c_str())) {
                for (const char* item: TRAM::animStates) {
                    const bool selected = (tram.lookup.toState == item);
                    if (ImGui::Selectable(item, selected)) {
                        tram.lookup.toState = item;
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // Row 3 — Variant / First Level
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Variant:");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##varient", tram.lookup.varient.c_str())) {
                for (const char* item: s_varients) {
                    const bool selected = (tram.lookup.varient == item);
                    if (ImGui::Selectable(item, selected)) {
                        tram.lookup.varient = item;
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::TableSetColumnIndex(3);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("First Level:");
            ImGui::TableSetColumnIndex(4);
            ImGui::SetNextItemWidth(-1); {
                char buf[32];
                std::snprintf(buf, sizeof(buf), "%s", tram.lookup.firstLevel.c_str());
                if (ImGui::InputText("##firstlevel", buf, sizeof(buf))) {
                    tram.lookup.firstLevel = buf;
                    if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                }
            }

            ImGui::EndTable();
        }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Flags ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Flags:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%s", tram.flags.c_str());
        if (ImGui::InputText("##flags", buf, sizeof(buf))) {
            tram.flags = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Atomic ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Atomic:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(80.0f); {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%s", tram.atomic.start.c_str());
        if (ImGui::InputText("##atomicstart", buf, sizeof(buf))) {
            tram.atomic.start = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }
    ImGui::SameLine();
    ImGui::TextUnformatted("->");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f); {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%s", tram.atomic.end.c_str());
        if (ImGui::InputText("##atomicend", buf, sizeof(buf))) {
            tram.atomic.end = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }

    // --- Invulnerable ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Invulnerable:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(80.0f); {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%s", tram.invulnerable.start.c_str());
        if (ImGui::InputText("##invulstart", buf, sizeof(buf))) {
            tram.invulnerable.start = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }
    ImGui::SameLine();
    ImGui::TextUnformatted("->");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f); {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%s", tram.invulnerable.end.c_str());
        if (ImGui::InputText("##invulend", buf, sizeof(buf))) {
            tram.invulnerable.end = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Direction ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Direction:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    if (ImGui::BeginCombo("##direction", tram.direction.c_str())) {
        for (const char* item: TRAM::animDirections) {
            const bool selected = (tram.direction == item);
            if (ImGui::Selectable(item, selected)) {
                tram.direction = item;
                if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
            }
            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Animation Data ---
    auto stripTram = [](const std::string& stem) -> std::string {
        if (stem.starts_with("TRAM")) return stem.substr(4);
        return stem;
    };

    const std::string currentAnimLabel = !m_selectedAnimName.empty()
                                             ? m_selectedAnimName
                                             : (tram.animationData
                                                    ? stripTram(file.name)
                                                    : "(DAE import)");

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Animation:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        static char animFilter[128] = {};
        static bool animWasOpen     = false;

        if (ImGui::BeginCombo("##animdata", currentAnimLabel.c_str())) {
            if (!animWasOpen) {
                animFilter[0] = '\0';
                animWasOpen   = true;
            }

            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##animfilter", animFilter, sizeof(animFilter));
            ImGui::Separator();

            const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 6.0f;
            ImGui::BeginChild("##animlist", {0, listHeight}, false);

            for (const auto& f_trac: m_vanilla.getTracFiles()) {
                const std::string displayName = stripTram(f_trac.name);
                if (animFilter[0] != '\0' && displayName.find(animFilter) == std::string::npos)
                    continue;
                const bool selected = (m_selectedAnimName == displayName);
                if (ImGui::Selectable(displayName.c_str(), selected)) {
                    // We need to find the data for this name
                    for (const auto& f: m_vanilla.getTramFiles()) {
                        if (f.name == f_trac.name) {
                            tram.animationData = f.data.animationData;
                            break;
                        }
                    }
                    tram.importPath    = std::nullopt;
                    m_selectedAnimName = displayName;
                    animFilter[0]      = '\0';
                    if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                    ImGui::CloseCurrentPopup();
                }
                if (selected) ImGui::SetItemDefaultFocus();
            }

            ImGui::EndChild();
            ImGui::EndCombo();
        } else {
            animWasOpen = false;
        }
    }

    // Show frame count from DTO — no XML parsing needed
    if (tram.animationData && tram.animationData->frameCount > 0) {
        ImGui::SameLine();
        ImGui::TextDisabled("(%d frames)", tram.animationData->frameCount);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Direct Animations ---
    ImGui::TextUnformatted("Direct Animations");
    ImGui::Separator();
    ImGui::Spacing();

    while (tram.directAnimations.size() < 2)
        tram.directAnimations.emplace_back("");

    for (int i = 0; i < 2; ++i) {
        ImGui::PushID(i);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(i == 0 ? "Link 1:" : "Link 2:");
        ImGui::SameLine(labelWidth);
        ImGui::SetNextItemWidth(fieldWidth); {
            static char filter[2][128] = {};
            static bool wasOpen[2]     = {};

            const std::string& current = tram.directAnimations[i];
            if (ImGui::BeginCombo("##link", current.c_str())) {
                if (!wasOpen[i]) {
                    filter[i][0] = '\0';
                    wasOpen[i]   = true;
                }
                ImGui::SetNextItemWidth(-1);
                ImGui::InputText("##filter", filter[i], sizeof(filter[i]));
                ImGui::Separator();

                const float listHeight = ImGui::GetTextLineHeightWithSpacing() * 6.0f;
                ImGui::BeginChild("##list", {0, listHeight}, false);

                if (ImGui::Selectable("(none)", current.empty())) {
                    tram.directAnimations[i] = "";
                    filter[i][0]             = '\0';
                    if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                }
                // Project files
                ImGui::SeparatorText("Project");
                for (const auto& f: m_project.getTramFiles()) {
                    if (filter[i][0] != '\0' && f.name.find(filter[i]) == std::string::npos)
                        continue;
                    if (f.name == current) continue;
                    if (ImGui::Selectable(f.name.c_str(), false)) {
                        tram.directAnimations[i] = f.name;
                        filter[i][0]             = '\0';
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                        ImGui::CloseCurrentPopup();
                    }
                }

                // Vanilla files
                ImGui::SeparatorText("Vanilla");
                for (const auto& f: m_vanilla.getTramFiles()) {
                    if (filter[i][0] != '\0' && f.name.find(filter[i]) == std::string::npos)
                        continue;
                    if (f.name == current) continue;
                    if (ImGui::Selectable(f.name.c_str(), false)) {
                        tram.directAnimations[i] = f.name;
                        filter[i][0]             = '\0';
                        if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::EndChild();
                ImGui::EndCombo();
            } else {
                wasOpen[i] = false;
            }
        }
        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Impact ---
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Impact:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%s", tram.impact.c_str());
        if (ImGui::InputText("##impact", buf, sizeof(buf))) {
            tram.impact = buf;
            if (file.status == FileStatus::Unmodified) file.status = FileStatus::Modified;
        }
    }
}

// ---------------------------------------------------------------------------
// Save
// ---------------------------------------------------------------------------

void TRAMView::saveWithRename(OniFile<TRAM::Root>& file, const int selectedIndex) {
    if (m_originalPaths.contains(selectedIndex)) {
        if (const auto& original = m_originalPaths[selectedIndex];
            original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalPaths[selectedIndex] = file.path;
    }
    m_project.saveFile(file);
    file.status = FileStatus::Unmodified;
}

// ---------------------------------------------------------------------------
// Helper
// ---------------------------------------------------------------------------

void TRAMView::renderCombo(const char*                     label, std::string& value,
                           const std::vector<const char*>& items,
                           const float                     labelWidth, const float fieldWidth) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);

    const std::string id = std::string("##") + label;
    if (ImGui::BeginCombo(id.c_str(), value.c_str())) {
        for (const char* item: items) {
            const bool selected = (value == item);
            if (ImGui::Selectable(item, selected)) value = item;
            if (selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}