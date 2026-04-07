#pragma once

#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"
#include "gui/OniGuiRenderer.hpp"
#include "gui/views/AddFileModal.hpp"
#include "gui/views/ONCCView.hpp"
#include "gui/views/ONCVView.hpp"
#include "gui/views/SettingsModal.hpp"
#include "gui/views/TRACView.hpp"
#include "gui/views/TRAMView.hpp"
#include "model/Config.hpp"
#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include "repository/TRACRepository.hpp"
#include "repository/TRAMRepository.hpp"
#include "service/OniSplitService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "service/VanillaCatalogService.hpp"

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

/**
 * @brief Main application class for OniForge.
 *
 * This class serves as the composition root of the application.
 * It owns all domain dependencies, the renderer, and the UI tabs.
 * It is responsible for the main loop, top-level layout, and coordinating
 * interactions between services and views.
 */
class OniForgeApp {
public:
    OniForgeApp();
    ~OniForgeApp();

    OniForgeApp(const OniForgeApp&) = delete;
    OniForgeApp& operator=(const OniForgeApp&) = delete;
    OniForgeApp(OniForgeApp&&) = delete;
    OniForgeApp& operator=(OniForgeApp&&) = delete;

    /**
     * @brief Starts the application main loop.
     *
     * Initializes the renderer, loads data, and enters the game loop.
     * Blocks until the application is closed.
     * @return Exit code (0 for success, non-zero for error).
     */
    int run();

private:
    static constexpr std::string_view CONFIG_FILE = "oniforge.config.xml";

    // --- Core ---
    OniForge::Config m_config;
    Logger           m_logger;
    XmlReader        m_reader;
    XmlWriter        m_writer;

    // --- Persistence ---
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
    ONCCView                 m_onccView;
    ONCVView                 m_oncvView;
    TRACView                 m_tracView;
    TRAMView                 m_tramView;
    AddFileModal             m_addFileModal;
    OniForge::SettingsModal  m_settingsModal;

    // --- App state ---
    bool  m_running           = false;
    int   m_selectedOnccIndex = -1;
    int   m_selectedOncvIndex = -1;
    int   m_selectedTracIndex = -1;
    int   m_selectedTramIndex = -1;

    // --- Try in ONI modal state ---
    bool                     m_showTryInOniModal = false;
    std::atomic<bool>        m_tryInOniRunning   = false;
    std::atomic<bool>        m_tryInOniSuccess   = false;
    std::vector<std::string> m_tryInOniLog;
    std::mutex               m_oniSplitLogMutex;
    std::thread              m_oniSplitThread;

    // --- Lifecycle ---
    
    /**
     * @brief Initializes the renderer and loads initial data.
     * @return True if initialization succeeded, false otherwise.
     */
    bool init();

    /**
     * @brief The core application loop (Input -> Update -> Render).
     */
    void mainLoop();
    void loadConfig();
    void saveConfig();

    // --- Render ---
    
    /**
     * @brief High-level render function called every frame.
     */
    void render();

    void renderMenuBar();
    void renderLeftPanel();
    void renderRightPanel();
    
    /**
     * @brief Renders the "Try in Oni" modal dialog if active.
     */
    void renderTryInOniModal();
};
