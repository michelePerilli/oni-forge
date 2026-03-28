#include "gui/OniForgeApp.hpp"
#include <imgui.h>
#include "gui/OniUI.hpp"
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
      , m_tramRepo(m_reader, m_writer, m_logger)
      , m_repos{m_onccRepo, m_oncvRepo, m_tracRepo, m_tramRepo}
      , m_vanilla(m_repos, m_logger)
      , m_project(m_repos, m_vanilla, m_logger)
      , m_oniSplit(m_config.oniSplitPath, m_config.oniGamePath)
      , m_onccView(m_vanilla, m_project)
      , m_oncvView(m_vanilla, m_project)
      , m_tracView(m_vanilla, m_project)
      , m_tramView(m_vanilla, m_project)
      , m_addFileModal(m_vanilla, m_project, m_logger)
      , m_settingsModal(m_config, m_renderer) {
    loadConfig();
}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

/**
 * @brief Main execution entry point for the application.
 * @return Exit code (0 for success).
 */
int OniForgeApp::run() {
    if (!init()) return 1;
    mainLoop();
    return 0;
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

/**
 * @brief Initializes application services, logs, and renderer.
 * @return True if initialization was successful.
 */
bool OniForgeApp::init() {
    m_logger.separator();
    m_logger.info("[OniForge] Initializing...");

    // Initial data load from configured paths
    m_vanilla.loadFromFolder(m_config.vanillaPath);
    m_project.loadFromFolder(m_config.projectPath);
    m_logger.separator();

    if (!m_renderer.init("OniForge", 1280, 720, m_config.theme, static_cast<float>(m_config.fontSize)))
        return false;

    m_running = true;
    return true;
}

/**
 * @brief Standard game loop: handles events, updates, and rendering.
 */
void OniForgeApp::mainLoop() {
    while (m_running) {
        // Deferred font reload must happen OUTSIDE of beginFrame/endFrame 
        // because ImGui forbids modifying the font atlas while a frame is active.
        if (m_renderer.isFontReloadPending()) {
            m_renderer.loadFont(m_renderer.getPendingFontSize());
            m_renderer.setFontReloadPending(false);
        }
        
        m_renderer.beginFrame(m_running);
        render();
        m_renderer.endFrame();
    }
}

/**
 * @brief Loads application settings from oniforge.config.xml.
 */
void OniForgeApp::loadConfig() {
    if (!std::filesystem::exists(std::string(CONFIG_FILE))) {
        m_logger.info("[Config] Config file not found, using defaults.");
        return;
    }

    XmlDocument doc;
    if (!m_reader.read(std::string(CONFIG_FILE), doc)) {
        m_logger.error("[Config] Failed to read config file.");
        return;
    }

    const auto root = doc.getRawDocument().child("OniForgeConfig");
    if (!root) return;

    m_config.vanillaPath  = root.child("VanillaPath").text().as_string();
    m_config.projectPath  = root.child("ProjectPath").text().as_string();
    m_config.oniSplitPath = root.child("OniSplitPath").text().as_string();
    m_config.oniGamePath  = root.child("OniGamePath").text().as_string();
    m_config.tempOniPath  = root.child("TempOniPath").text().as_string();
    m_config.theme        = static_cast<Theme>(root.child("Theme").text().as_int());
    m_config.fontSize     = root.child("FontSize").text().as_int();

    m_logger.info("[Config] Configuration loaded.");
}

/**
 * @brief Persists current application settings to oniforge.config.xml.
 */
void OniForgeApp::saveConfig() {
    XmlDocument doc;
    auto        root = doc.getRawDocument().append_child("OniForgeConfig");

    root.append_child("VanillaPath").text().set(m_config.vanillaPath.c_str());
    root.append_child("ProjectPath").text().set(m_config.projectPath.c_str());
    root.append_child("OniSplitPath").text().set(m_config.oniSplitPath.c_str());
    root.append_child("OniGamePath").text().set(m_config.oniGamePath.c_str());
    root.append_child("TempOniPath").text().set(m_config.tempOniPath.c_str());
    root.append_child("Theme").text().set(static_cast<int>(m_config.theme));
    root.append_child("FontSize").text().set(m_config.fontSize);

    // markAsLoaded() is required so the XmlWriter accepts the manually built tree
    doc.markAsLoaded();

    if (m_writer.write(doc, std::string(CONFIG_FILE))) {
        m_logger.info("[Config] Configuration saved.");
    } else {
        m_logger.error("[Config] Failed to save configuration.");
    }
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

/**
 * @brief Main UI composition method.
 */
void OniForgeApp::render() {
    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##root", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
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
    m_settingsModal.render([this]() {
        saveConfig();
        // Re-init OniSplit if paths changed
        m_oniSplit = OniSplitService(m_config.oniSplitPath, m_config.oniGamePath);
    });
    renderTryInOniModal();
}

/**
 * @brief Renders the application's top menu bar.
 */
void OniForgeApp::renderMenuBar() {
    if (!ImGui::BeginMenuBar()) return;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save All"))
            m_project.saveToFolder(m_config.projectPath);
        ImGui::Separator();
        if (ImGui::MenuItem("Settings..."))
            m_settingsModal.open();
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
            m_running = false;
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Project")) {
        if (ImGui::MenuItem("Try in ONI")) {
            m_tryInOniLog.clear();
            m_tryInOniRunning   = true;
            m_tryInOniSuccess   = false;
            m_showTryInOniModal = true;

            m_project.saveToFolder(m_config.projectPath);
            m_tryInOniLog.emplace_back("[OniForge] Project saved.");

            m_tryInOniSuccess = m_oniSplit.tryInOni(
                m_config.projectPath,
                m_config.tempOniPath,
                false,
                [this](const std::string& line) { m_tryInOniLog.push_back(line); }
            );
            m_tryInOniRunning = false;
        }
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}

/**
 * @brief Renders the modal window showing progress of the "Try in ONI" task.
 */
void OniForgeApp::renderTryInOniModal() {
    if (!m_showTryInOniModal) return;

    ImGui::OpenPopup("Try in ONI");

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSize({700, 450}, ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        {io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f},
        ImGuiCond_Always, {0.5f, 0.5f}
    );

    if (!ImGui::BeginPopupModal("Try in ONI", &m_showTryInOniModal,
                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        return;

    if (m_tryInOniRunning)
        ImGui::TextUnformatted("Running...");
    else if (m_tryInOniSuccess)
        ImGui::TextColored({0.4f, 0.9f, 0.4f, 1.0f}, "Success!");
    else
        ImGui::TextColored({0.9f, 0.3f, 0.3f, 1.0f}, "Failed.");

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("##log", {0, 350}, true, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line: m_tryInOniLog)
        ImGui::TextUnformatted(line.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::Spacing();
    if (!m_tryInOniRunning) {
        if (ImGui::Button("Close", {120, 0})) {
            m_showTryInOniModal = false;
            ImGui::CloseCurrentPopup();
        }
    }

    ImGui::EndPopup();
}

/**
 * @brief Renders the left sidebar containing the project file tree.
 */
void OniForgeApp::renderLeftPanel() {
    ImGui::TextDisabled("Project Contents");
    ImGui::Separator();

    if (const auto& onccFiles = m_project.getOnccFiles(); !onccFiles.empty()) {
        const std::string header = "ONCC (" + std::to_string(onccFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(onccFiles.size()); ++i) {
                const std::string& name = onccFiles[i].name;
                OniUI::PushFileStatusColor(onccFiles[i].status);
                if (const bool selected = (m_selectedOnccIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOnccIndex = i;
                    m_selectedOncvIndex = -1;
                    m_selectedTracIndex = -1;
                    m_selectedTramIndex = -1;
                }
                OniUI::PopFileStatusColor();
            }
        }
    }

    if (const auto& oncvFiles = m_project.getOncvFiles(); !oncvFiles.empty()) {
        const std::string header = "ONCV (" + std::to_string(oncvFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(oncvFiles.size()); ++i) {
                const std::string& name = oncvFiles[i].name;
                OniUI::PushFileStatusColor(oncvFiles[i].status);
                if (const bool selected = (m_selectedOncvIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOncvIndex = i;
                    m_selectedOnccIndex = -1;
                    m_selectedTracIndex = -1;
                    m_selectedTramIndex = -1;
                }
                OniUI::PopFileStatusColor();
            }
        }
    }

    if (const auto& tracFiles = m_project.getTracFiles(); !tracFiles.empty()) {
        const std::string header = "TRAC (" + std::to_string(tracFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(tracFiles.size()); ++i) {
                const std::string& name = tracFiles[i].name;
                OniUI::PushFileStatusColor(tracFiles[i].status);
                if (const bool selected = (m_selectedTracIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedTracIndex = i;
                    m_selectedOnccIndex = -1;
                    m_selectedOncvIndex = -1;
                    m_selectedTramIndex = -1;
                    m_tracView.onFileChanged();
                }
                OniUI::PopFileStatusColor();
            }
        }
    }

    if (const auto& tramFiles = m_project.getTramFiles(); !tramFiles.empty()) {
        const std::string header = "TRAM (" + std::to_string(tramFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(tramFiles.size()); ++i) {
                const std::string& name = tramFiles[i].name;
                OniUI::PushFileStatusColor(tramFiles[i].status);
                if (const bool selected = (m_selectedTramIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedTramIndex = i;
                    m_selectedOnccIndex = -1;
                    m_selectedOncvIndex = -1;
                    m_selectedTracIndex = -1;
                    m_tramView.onFileChanged();
                }
                OniUI::PopFileStatusColor();
            }
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 36.0f);
    ImGui::Separator();
    if (ImGui::Button("+ Add file..", {-1, 0}))
        m_addFileModal.open();
}

/**
 * @brief Renders the right panel content based on the currently selected file.
 */
void OniForgeApp::renderRightPanel() {
    if (m_selectedOnccIndex >= 0 &&
        m_selectedOnccIndex < static_cast<int>(m_project.getOnccFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCC::Root>>&>(m_project.getOnccFiles());
        auto& file  = files[m_selectedOnccIndex];
        m_onccView.renderHeaderRow(file, m_selectedOnccIndex);
        if (ImGui::BeginTabBar("##tabs")) {
            m_onccView.render(file, m_selectedOnccIndex);
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedOncvIndex >= 0 &&
        m_selectedOncvIndex < static_cast<int>(m_project.getOncvFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCV::Root>>&>(m_project.getOncvFiles());
        auto& file  = files[m_selectedOncvIndex];
        m_oncvView.renderHeaderRow(file, m_selectedOncvIndex);
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                m_oncvView.render(file, m_selectedOncvIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedTracIndex >= 0 &&
        m_selectedTracIndex < static_cast<int>(m_project.getTracFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<TRAC::Root>>&>(m_project.getTracFiles());
        auto& file  = files[m_selectedTracIndex];
        m_tracView.renderHeaderRow(file, m_selectedTracIndex);
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                m_tracView.render(file, m_selectedTracIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedTramIndex >= 0 &&
        m_selectedTramIndex < static_cast<int>(m_project.getTramFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<TRAM::Root>>&>(m_project.getTramFiles());
        auto& file  = files[m_selectedTramIndex];
        m_tramView.renderHeaderRow(file, m_selectedTramIndex);
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("Animation")) {
                m_tramView.render(file, m_selectedTramIndex);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    ImGui::TextDisabled("Select a file from the project list.");
}
