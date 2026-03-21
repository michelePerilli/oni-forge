#include "gui/OniForgeApp.hpp"

#include <imgui.h>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

OniForgeApp::OniForgeApp()
    : m_logger("oniforge.log")
    , m_reader(m_logger)
    , m_writer(m_logger)
    , m_onccRepo(m_reader, m_writer, m_logger)
    , m_oncvRepo(m_reader, m_writer, m_logger)
    , m_tracRepo(m_reader, m_writer, m_logger)
    , m_repos{ m_onccRepo, m_oncvRepo, m_tracRepo }
    , m_vanilla(m_repos, m_logger)
    , m_project(m_repos, m_vanilla, m_logger)
    , m_onccView(m_vanilla, m_project)
    , m_oncvView(m_vanilla, m_project)
    , m_tracView(m_vanilla, m_project)
    , m_addFileModal(m_vanilla, m_project, m_logger) {}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

int OniForgeApp::run() {
    if (!init()) return 1;
    mainLoop();
    return 0;
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

bool OniForgeApp::init() {
    m_logger.separator();
    m_vanilla.loadFromFolder(std::string(VANILLA_PATH));
    m_project.loadFromFolder(std::string(PROJECT_PATH));
    m_logger.separator();

    if (!m_renderer.init("OniForge", 1280, 720, m_currentTheme, FONT_SIZES[m_currentFontIndex]))
        return false;

    m_running = true;
    return true;
}

void OniForgeApp::mainLoop() {
    while (m_running) {
        if (m_fontReloadPending) {
            m_renderer.loadFont(m_pendingFontSize);
            m_fontReloadPending = false;
        }

        m_renderer.beginFrame(m_running);
        render();
        m_renderer.endFrame();
    }
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

void OniForgeApp::render() {
    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##root", nullptr,
        ImGuiWindowFlags_NoTitleBar  |
        ImGuiWindowFlags_NoResize    |
        ImGuiWindowFlags_NoMove      |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_MenuBar
    );

    renderMenuBar();

    constexpr float leftWidth  = 250.0f;
    const float     rightWidth = ImGui::GetContentRegionAvail().x - leftWidth - 8.0f;

    ImGui::BeginChild("##left", {leftWidth, 0}, true);
    renderLeftPanel();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##right", {rightWidth, 0}, true);
    renderRightPanel();
    ImGui::EndChild();

    ImGui::End();

    m_addFileModal.render();
}

void OniForgeApp::renderMenuBar() {
    if (!ImGui::BeginMenuBar()) return;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save All"))
            m_project.saveToFolder(std::string(PROJECT_PATH));
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
            m_running = false;
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::BeginMenu("Theme")) {
            if (ImGui::MenuItem("Dark",    nullptr, m_currentTheme == Theme::Dark))    { m_renderer.applyTheme(Theme::Dark);    m_currentTheme = Theme::Dark; }
            if (ImGui::MenuItem("Light",   nullptr, m_currentTheme == Theme::Light))   { m_renderer.applyTheme(Theme::Light);   m_currentTheme = Theme::Light; }
            if (ImGui::MenuItem("Classic", nullptr, m_currentTheme == Theme::Classic)) { m_renderer.applyTheme(Theme::Classic); m_currentTheme = Theme::Classic; }
            if (ImGui::MenuItem("Neutral", nullptr, m_currentTheme == Theme::Neutral)) { m_renderer.applyTheme(Theme::Neutral); m_currentTheme = Theme::Neutral; }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Font Size")) {
            for (int i = 0; i < FONT_COUNT; ++i) {
                if (ImGui::MenuItem(FONT_LABELS[i], nullptr, m_currentFontIndex == i)) {
                    m_currentFontIndex  = i;
                    m_pendingFontSize   = FONT_SIZES[i];
                    m_fontReloadPending = true;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}

void OniForgeApp::renderLeftPanel() {
    ImGui::TextDisabled("Project");
    ImGui::Separator();

    if (const auto& onccFiles = m_project.getOnccFiles(); !onccFiles.empty()) {
        const std::string header = "ONCC (" + std::to_string(onccFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(onccFiles.size()); ++i) {
                const std::string name = onccFiles[i].path.stem().string();
                if (const bool selected = (m_selectedOnccIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOnccIndex = i;
                    m_selectedOncvIndex = -1;
                    m_selectedTracIndex = -1;
                }
            }
        }
    }

    if (const auto& oncvFiles = m_project.getOncvFiles(); !oncvFiles.empty()) {
        const std::string header = "ONCV (" + std::to_string(oncvFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(oncvFiles.size()); ++i) {
                const std::string name = oncvFiles[i].path.stem().string();
                if (const bool selected = (m_selectedOncvIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOncvIndex = i;
                    m_selectedOnccIndex = -1;
                    m_selectedTracIndex = -1;
                }
            }
        }
    }

    if (const auto& tracFiles = m_project.getTracFiles(); !tracFiles.empty()) {
        const std::string header = "TRAC (" + std::to_string(tracFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(tracFiles.size()); ++i) {
                const std::string name = tracFiles[i].path.stem().string();
                if (const bool selected = (m_selectedTracIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedTracIndex = i;
                    m_selectedOnccIndex = -1;
                    m_selectedOncvIndex = -1;
                    m_tracView.onFileChanged();
                }
            }
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 36.0f);
    ImGui::Separator();
    if (ImGui::Button("+ Add file..", {-1, 0}))
        m_addFileModal.open();
}

void OniForgeApp::renderRightPanel() {
    if (m_selectedOnccIndex >= 0 &&
        m_selectedOnccIndex < static_cast<int>(m_project.getOnccFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCC::Root>>&>(m_project.getOnccFiles());
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                m_onccView.render(files[m_selectedOnccIndex], m_selectedOnccIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedOncvIndex >= 0 &&
        m_selectedOncvIndex < static_cast<int>(m_project.getOncvFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCV::Root>>&>(m_project.getOncvFiles());
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                m_oncvView.render(files[m_selectedOncvIndex], m_selectedOncvIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedTracIndex >= 0 &&
        m_selectedTracIndex < static_cast<int>(m_project.getTracFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<TRAC::Root>>&>(m_project.getTracFiles());
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                m_tracView.render(files[m_selectedTracIndex], m_selectedTracIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    ImGui::TextDisabled("Select a file from the project list.");
}
