#pragma once

#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"
#include "gui/OniGuiRenderer.hpp"
#include "gui/OniForgeTheme.hpp"
#include "model/ONCC.hpp"
#include "model/ONCV.hpp"
#include "model/OniFile.hpp"
#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include "repository/TRACRepository.hpp"
#include "service/ProjectCatalogService.hpp"
#include "service/VanillaCatalogService.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief Main application class for OniForge.
 *
 * Owns all domain dependencies and the ImGuiRenderer.
 * Responsible for the main loop and all render logic.
 * Does not know anything about SDL2 or OpenGL directly.
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
    static constexpr std::string_view VANILLA_PATH = R"(D:\Dev\java\oniforge\vanilla\xml)";
    static constexpr std::string_view PROJECT_PATH = R"(D:\Dev\mods\oni\TCTFagent)";

    static constexpr float       FONT_SIZES[]  = { 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f };
    static constexpr const char* FONT_LABELS[] = { "13",  "14",  "15",  "16",  "17",  "18"  };
    static constexpr int         FONT_COUNT    = 6;
    static constexpr int         FONT_DEFAULT  = 2;

    // --- Renderer ---
    OniGuiRenderer m_renderer;

    // --- Infrastructure ---
    Logger                m_logger;
    XmlReader             m_reader;
    XmlWriter             m_writer;
    ONCCRepository        m_onccRepo;
    ONCVRepository        m_oncvRepo;
    TRACRepository        m_tracRepo;
    OniRepositoryRegistry m_repos;

    // --- Services ---
    VanillaCatalogService m_vanilla;
    ProjectCatalogService m_project;

    // --- App state ---
    bool  m_running           = false;
    int   m_selectedOnccIndex = -1;
    int   m_selectedOncvIndex = -1;
    Theme m_currentTheme      = Theme::Neutral;
    int   m_currentFontIndex  = FONT_DEFAULT;
    bool  m_fontReloadPending = false;
    float m_pendingFontSize   = 15.0f;

    // --- Original paths for rename tracking ---
    std::unordered_map<int, std::filesystem::path> m_originalOnccPaths;
    std::unordered_map<int, std::filesystem::path> m_originalOncvPaths;

    // --- Add file modal state ---
    bool                            m_showAddFileModal = false;
    std::unordered_set<std::string> m_pendingAddFiles;

    // --- Lifecycle ---
    bool init();
    void mainLoop();

    // --- Render ---
    void render();
    void renderMenuBar();
    void renderLeftPanel();
    void renderRightPanel();
    void renderGeneralTab(OniFile<ONCC::Root>& file);
    void renderOncvTab(OniFile<ONCV::Root>& file);
    void renderAddFileModal();

    // --- Helpers ---
    void snapshotOriginalPaths();
    void saveWithRename(OniFile<ONCC::Root>& file);
    void saveWithRename(OniFile<ONCV::Root>& file);
    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;
    [[nodiscard]] std::vector<std::string> getVanillaOncvNamesStripped() const;
    [[nodiscard]] std::vector<std::string> getVanillaTracNames() const;
    [[nodiscard]] bool isInProject(const std::string& stem) const;
};
