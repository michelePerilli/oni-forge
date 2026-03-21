#include "gui/views/AddFileModal.hpp"

#include <imgui.h>

AddFileModal::AddFileModal(const VanillaCatalogService& vanilla,
                           ProjectCatalogService&       project,
                           const ILogger&               logger)
    : m_vanilla(vanilla)
      , m_project(project)
      , m_logger(logger) {
}

void AddFileModal::open() {
    m_pendingFiles.clear();
    m_justConfirmed = false;
    m_open          = true;
}

void AddFileModal::render() {
    m_justConfirmed = false;
    if (!m_open) return;

    ImGui::OpenPopup("Add file from vanilla");

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSize({500, 520}, ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        {io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f},
        ImGuiCond_Always, {0.5f, 0.5f}
    );

    if (!ImGui::BeginPopupModal("Add file from vanilla", &m_open,
                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        return;

    ImGui::TextDisabled("Select files to add to the project.");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("##filelist", {0, 380}, false);

    auto renderGroup = [&](const char* label, const auto& files) {
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& file: files) {
                const std::string stem      = file.path.stem().string();
                bool              checked   = m_pendingFiles.contains(stem);
                const bool        inProject = isInProject(stem);
                if (inProject) ImGui::BeginDisabled();
                if (ImGui::Checkbox(("##" + stem).c_str(), &checked)) {
                    if (checked) m_pendingFiles.insert(stem);
                    else m_pendingFiles.erase(stem);
                }
                ImGui::SameLine();
                ImGui::TextUnformatted(stem.c_str());
                if (inProject) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("(in project)");
                    ImGui::EndDisabled();
                }
            }
        }
    };

    renderGroup("ONCC", m_vanilla.getOnccFiles());
    renderGroup("ONCV", m_vanilla.getOncvFiles());
    renderGroup("TRAC", m_vanilla.getTracFiles());

    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Spacing();

    const std::string addLabel = m_pendingFiles.empty()
                                     ? "Add"
                                     : "Add (" + std::to_string(m_pendingFiles.size()) + ")";

    const bool canAdd = !m_pendingFiles.empty();
    if (!canAdd) ImGui::BeginDisabled();
    if (ImGui::Button(addLabel.c_str(), {120, 0})) {
        for (const auto& stem: m_pendingFiles) {
            if (!m_project.createOnccFromVanilla(stem))
                m_logger.warning("[UI] Failed to add ONCC: " + stem);
            if (!m_project.createOncvFromVanilla(stem))
                m_logger.warning("[UI] Failed to add ONCV: " + stem);
            if (!m_project.createTracFromVanilla(stem))
                m_logger.warning("[UI] Failed to add TRAC: " + stem);
        }
        m_pendingFiles.clear();
        m_justConfirmed = true;
        m_open          = false;
        ImGui::CloseCurrentPopup();
    }
    if (!canAdd) ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Cancel", {120, 0})) {
        m_pendingFiles.clear();
        m_open = false;
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

bool AddFileModal::isInProject(const std::string& stem) const {
    for (const auto& [path, data]: m_project.getOnccFiles())
        if (path.stem().string() == stem) return true;
    for (const auto& [path, data]: m_project.getOncvFiles())
        if (path.stem().string() == stem) return true;
    for (const auto& [path, data]: m_project.getTracFiles())
        if (path.stem().string() == stem) return true;
    return false;
}
