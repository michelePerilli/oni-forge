#pragma once

#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"
#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/TRACRepository.hpp"
#include "repository/TRAMRepository.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "service/OniSplitService.hpp"
#include "gui/OniGuiRenderer.hpp"
#include "gui/OniForgeTheme.hpp"
#include "gui/views/ONCCView.hpp"
#include "gui/views/ONCVView.hpp"
#include "gui/views/TRACView.hpp"
#include "gui/views/TRAMView.hpp"
#include "gui/views/AddFileModal.hpp"

#include <string>
#include <vector>

/**
 * @brief Main application class for OniForge.
 *
 * Owns all domain dependencies, the renderer and the UI tabs.
 * Responsible only for the main loop, left panel and menu bar.
 */
class OniForgeApp {
public:
    OniForgeApp();
    ~OniForgeApp() = default;

    OniForgeApp(const OniForgeApp&) = delete;
    OniForgeApp& operator=(const OniForgeApp&) = delete;
    OniForgeApp(OniForgeApp&&) = delete;
    OniForgeApp& operator=(OniForgeApp&&) = delete;

    int run();

private:
    static constexpr std::string_view VANILLA_PATH  = R"(D:\Dev\java\oniforge\vanilla\xml)";
    static constexpr std::string_view PROJECT_PATH  = R"(D:\Dev\mods\oni\TCTFagent)";
    static constexpr std::string_view ONISPLIT_PATH = R"(D:\Dev\mods\oni\.tools\OniSplit.exe)";
    static constexpr std::string_view ONI_GAME_PATH = R"(D:\Program Files (x86)\Oni\AE)";
    static constexpr std::string_view TEMP_ONI_PATH = R"(D:\Dev\mods\oni\TCTFagent\_build)";

    static constexpr float       FONT_SIZES[]  = { 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f };
    static constexpr const char* FONT_LABELS[] = { "13",  "14",  "15",  "16",  "17",  "18"  };
    static constexpr int         FONT_COUNT    = 6;
    static constexpr int         FONT_DEFAULT  = 2;

    // --- Infrastructure ---
    Logger                m_logger;
    XmlReader             m_reader;
    XmlWriter             m_writer;
    ONCCRepository        m_onccRepo;
    ONCVRepository        m_oncvRepo;
    TRACRepository        m_tracRepo;
    TRAMRepository        m_tramRepo;
    OniRepositoryRegistry m_repos;

    // --- Services ---
    VanillaCatalogService m_vanilla;
    ProjectCatalogService m_project;
    OniSplitService       m_oniSplit;

    // --- Renderer ---
    OniGuiRenderer m_renderer;

    // --- Views ---
    ONCCView     m_onccView;
    ONCVView     m_oncvView;
    TRACView     m_tracView;
    TRAMView     m_tramView;
    AddFileModal m_addFileModal;

    // --- App state ---
    bool  m_running           = false;
    int   m_selectedOnccIndex = -1;
    int   m_selectedOncvIndex = -1;
    int   m_selectedTracIndex = -1;
    int   m_selectedTramIndex = -1;
    Theme m_currentTheme      = Theme::Neutral;
    int   m_currentFontIndex  = FONT_DEFAULT;
    bool  m_fontReloadPending = false;
    float m_pendingFontSize   = 15.0f;

    // --- Try in ONI modal state ---
    bool                     m_showTryInOniModal = false;
    bool                     m_tryInOniRunning   = false;
    bool                     m_tryInOniSuccess   = false;
    std::vector<std::string> m_tryInOniLog;

    // --- Lifecycle ---
    bool init();
    void mainLoop();

    // --- Render ---
    void render();
    void renderMenuBar();
    void renderLeftPanel();
    void renderRightPanel();
    void renderTryInOniModal();
};