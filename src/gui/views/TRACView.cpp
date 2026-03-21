#include "gui/views/TRACView.hpp"

#include <imgui.h>
#include <algorithm>
#include <filesystem>
#include <cstring>

TRACView::TRACView(ProjectCatalogService& project)
    : m_project(project) {
}

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
    ImGui::SetNextItemWidth(fieldWidth); {
        char              buf[256];
        const std::string stem = file.path.stem().string();
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##tracname", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##trac", {60, 0}))
        saveWithRename(file, selectedIndex);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Parent Collection
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Parent Collection:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth); {
        char buf[256];
        strncpy(buf, parentCollection.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##parentcollection", buf, sizeof(buf)))
            parentCollection = buf;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // --- Animations header: label + count + actions ---
    const int  animCount   = static_cast<int>(animations.size());
    const bool anySelected = !m_selectedAnimIndices.empty();

    // --- Animations header: label + count on left, buttons on right ---
    constexpr float checkWidth  = 24.0f;
    constexpr float weightWidth = 70.0f;
    constexpr float btnWidth    = 60.0f;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Animations");
    ImGui::SameLine();
    ImGui::TextDisabled("(%d)", animCount);

    // Push buttons to the right
    ImGui::SameLine(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - btnWidth * 2 - 8.0f);

    if (ImGui::Button("+ Add", {btnWidth, 0})) {
        animations.push_back({"1", ""});
        m_selectedAnimIndices.clear();
        m_selectedAnimIndices.insert(animCount);
    }
    ImGui::SameLine();

    if (!anySelected) ImGui::BeginDisabled();
    if (ImGui::Button("Delete", {btnWidth, 0})) {
        std::vector<int> toRemove(m_selectedAnimIndices.begin(), m_selectedAnimIndices.end());
        std::ranges::sort(toRemove, std::greater<int>());
        for (const int idx: toRemove)
            if (idx < static_cast<int>(animations.size()))
                animations.erase(animations.begin() + idx);
        m_selectedAnimIndices.clear();
    }
    if (!anySelected) ImGui::EndDisabled();

    ImGui::Spacing();

    // --- Scrollable animation list ---

    // Column headers — prima riga, dentro il child
    const float padding       = ImGui::GetStyle().WindowPadding.x;
    const float checkboxWidth = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.x;
    const float animStart     = padding + checkboxWidth + weightWidth + ImGui::GetStyle().ItemSpacing.x * 2;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding + checkboxWidth);
    ImGui::TextDisabled("Weight");
    ImGui::SameLine(animStart);
    ImGui::TextDisabled("Animation");

    ImGui::BeginChild("##animlist", {0, ImGui::GetContentRegionAvail().y}, true);

    for (int i = 0; i < static_cast<int>(animations.size()); ++i) {
        auto& [weight, animation] = animations[i];
        ImGui::PushID(i);

        // Checkbox for multi-select
        bool checked = m_selectedAnimIndices.contains(i);
        if (ImGui::Checkbox("##sel", &checked)) {
            if (checked) m_selectedAnimIndices.insert(i);
            else m_selectedAnimIndices.erase(i);
        }
        ImGui::SameLine();

        // Weight field
        ImGui::SetNextItemWidth(weightWidth); {
            char buf[32];
            strncpy(buf, weight.c_str(), sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            if (ImGui::InputText("##w", buf, sizeof(buf)))
                weight = buf;
        }
        ImGui::SameLine();

        // Animation field — fills remaining width
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x); {
            char buf[256];
            strncpy(buf, animation.c_str(), sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            if (ImGui::InputText("##a", buf, sizeof(buf)))
                animation = buf;
        }

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
