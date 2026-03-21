#include "gui/views/TRACView.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <imgui.h>
#include <unordered_map>

TRACView::TRACView(const VanillaCatalogService& vanilla,
                   ProjectCatalogService&       project)
    : m_vanilla(vanilla)
    , m_project(project) {}

void TRACView::render(OniFile<TRAC::Root>& file, const int selectedIndex) {
    if (!m_originalPaths.contains(selectedIndex))
        m_originalPaths[selectedIndex] = file.path;

    auto& [id, parentCollection, animations] = file.data;

    constexpr float labelWidth = 140.0f;
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
        if (ImGui::InputText("##tracname", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##trac", {60, 0}))
        saveWithRename(file, selectedIndex);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // Parent Collection — combo with vanilla TRAC list
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Parent Collection:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const std::string& current = parentCollection;
        if (ImGui::BeginCombo("##parentcollection", current.c_str())) {
            for (const auto& [path, data] : m_vanilla.getTracFiles()) {
                const std::string name     = path.stem().string();
                const bool        selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) parentCollection = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // --- Animations header ---
    const int  animCount   = static_cast<int>(animations.size());
    const bool anySelected = !m_selectedAnimIndices.empty();

    constexpr float weightWidth = 70.0f;
    constexpr float btnWidth    = 60.0f;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Animations");
    ImGui::SameLine();
    ImGui::TextDisabled("(%d)", animCount);

    ImGui::SameLine(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - btnWidth * 2 - 8.0f);

    if (ImGui::Button("+ Add", {btnWidth, 0})) {
        animations.push_back({ "1", "" });
        m_selectedAnimIndices.clear();
        m_selectedAnimIndices.insert(animCount);
    }
    ImGui::SameLine();

    if (!anySelected) ImGui::BeginDisabled();
    if (ImGui::Button("Delete", {btnWidth, 0})) {
        std::vector<int> toRemove(m_selectedAnimIndices.begin(), m_selectedAnimIndices.end());
        std::ranges::sort(toRemove, std::greater<int>());
        for (const int idx : toRemove)
            if (idx < static_cast<int>(animations.size()))
                animations.erase(animations.begin() + idx);
        m_selectedAnimIndices.clear();
    }
    if (!anySelected) ImGui::EndDisabled();

    ImGui::Spacing();

    // --- Scrollable animation list ---
    ImGui::BeginChild("##animlist", {0, ImGui::GetContentRegionAvail().y}, true);

    // Column headers — inside child for correct alignment
    const float padding       = ImGui::GetStyle().WindowPadding.x;
    const float checkboxWidth = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.x;
    const float animStart     = padding + checkboxWidth + weightWidth + ImGui::GetStyle().ItemSpacing.x * 2;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding + checkboxWidth);
    ImGui::TextDisabled("Weight");
    ImGui::SameLine(animStart);
    ImGui::TextDisabled("Animation");
    ImGui::Separator();

    // Count occurrences for duplicate detection
    std::unordered_map<std::string, int> animOccurrences;
    for (const auto& a : animations)
        animOccurrences[a.animation]++;

    for (int i = 0; i < static_cast<int>(animations.size()); ++i) {
        TRAC::TRACAnimation& anim = animations[i];
        ImGui::PushID(i);

        // Checkbox for multi-select
        bool checked = m_selectedAnimIndices.contains(i);
        if (ImGui::Checkbox("##sel", &checked)) {
            if (checked) m_selectedAnimIndices.insert(i);
            else         m_selectedAnimIndices.erase(i);
        }
        ImGui::SameLine();

        // Weight field
        ImGui::SetNextItemWidth(weightWidth);
        {
            char buf[32];
            strncpy(buf, anim.weight.c_str(), sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            if (ImGui::InputText("##w", buf, sizeof(buf)))
                anim.weight = buf;
        }
        ImGui::SameLine();

        // Animation combo — red if duplicate
        const bool isDuplicate = !anim.animation.empty() && animOccurrences[anim.animation] > 1;
        if (isDuplicate) {
            ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.6f, 0.15f, 0.15f, 1.0f));
        }

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        {
            const std::string& current = anim.animation;
            if (ImGui::BeginCombo("##a", current.c_str())) {
                for (const auto& [path, data] : m_vanilla.getTracFiles()) {
                    const std::string name     = path.stem().string();
                    const bool        selected = (name == current);
                    if (ImGui::Selectable(name.c_str(), selected)) anim.animation = name;
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        if (isDuplicate) ImGui::PopStyleColor(2);

        ImGui::PopID();
    }

    ImGui::EndChild();
}

void TRACView::saveWithRename(const OniFile<TRAC::Root>& file, const int selectedIndex) {
    if (m_originalPaths.contains(selectedIndex)) {
        if (const auto& original = m_originalPaths[selectedIndex];
            original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalPaths[selectedIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}