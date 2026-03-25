#include "gui/views/SettingsModal.hpp"
#include <imgui.h>

namespace OniForge {

SettingsModal::SettingsModal(Config& config, OniGuiRenderer& renderer)
    : m_config(config), m_renderer(renderer) {
    updateFontIndex();
}

void SettingsModal::open() {
    m_isOpen = true;
    updateFontIndex();
}

void SettingsModal::updateFontIndex() {
    constexpr int FONT_COUNT = 6;
    constexpr int FONT_SIZES[] = { 13, 14, 15, 16, 17, 18 };
    
    m_selectedFontIndex = 2; // Default 15
    for (int i = 0; i < FONT_COUNT; ++i) {
        if (FONT_SIZES[i] == m_config.fontSize) {
            m_selectedFontIndex = i;
            break;
        }
    }
}

void SettingsModal::render(const std::function<void()>& onSave) {
    if (!m_isOpen) return;

    ImGui::OpenPopup("Settings");

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSize({650, 480}, ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        {io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f},
        ImGuiCond_Always, {0.5f, 0.5f}
    );

    if (!ImGui::BeginPopupModal("Settings", &m_isOpen,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        return;

    ImGui::TextDisabled("Paths");
    ImGui::Separator();
    ImGui::Spacing();

    auto pathInput = [](const char* label, std::string& path) {
        char buf[512];
        strncpy(buf, path.c_str(), sizeof(buf));
        if (ImGui::InputText(label, buf, sizeof(buf))) {
            path = buf;
            return true;
        }
        return false;
    };

    pathInput("Vanilla XML Folder", m_config.vanillaPath);
    pathInput("Project Folder",     m_config.projectPath);
    pathInput("OniSplit Executable",m_config.oniSplitPath);
    pathInput("Oni Game Folder",    m_config.oniGamePath);
    pathInput("Temp Build Folder",  m_config.tempOniPath);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::TextDisabled("Appearance");
    ImGui::Separator();
    ImGui::Spacing();

    const char* themes[] = { "Neutral", "Dark", "Light", "Classic" };
    int currentTheme = static_cast<int>(m_config.theme);
    if (ImGui::Combo("Theme", &currentTheme, themes, IM_ARRAYSIZE(themes))) {
        m_config.theme = static_cast<Theme>(currentTheme);
        m_renderer.applyTheme(m_config.theme);
    }

    const char* fontSizes[] = { "13", "14", "15", "16", "17", "18" };
    if (ImGui::Combo("Font Size", &m_selectedFontIndex, fontSizes, IM_ARRAYSIZE(fontSizes))) {
        constexpr int FONT_SIZES[] = { 13, 14, 15, 16, 17, 18 };
        m_config.fontSize = FONT_SIZES[m_selectedFontIndex];
        // Signal a font reload outside of the frame
        m_renderer.setFontReloadPending(true, static_cast<float>(m_config.fontSize));
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40.0f);
    ImGui::Separator();
    
    if (ImGui::Button("Save", {120, 0})) {
        if (onSave) onSave();
        m_isOpen = false;
        ImGui::CloseCurrentPopup();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Cancel", {120, 0})) {
        m_isOpen = false;
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

} // namespace OniForge
