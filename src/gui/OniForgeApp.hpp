#pragma once

#include "component/logger/Logger.hpp"
#include "component/xml/XmlReader.hpp"
#include "component/xml/XmlWriter.hpp"
#include "repository/ONCCRepository.hpp"
#include "repository/ONCVRepository.hpp"
#include "repository/TRACRepository.hpp"
#include "repository/OniRepositoryRegistry.hpp"
#include "service/VanillaCatalogService.hpp"
#include "service/ProjectCatalogService.hpp"
#include "model/OniFile.hpp"
#include "model/ONCC.hpp"

#include <string>
#include <vector>

struct SDL_Window;
typedef void* SDL_GLContext;



// Dopo gli altri include
enum class Theme { Dark, Light, Classic };

/**
 * @brief Main application class for OniForge.
 *
 * Owns all dependencies and manages the SDL2 window,
 * OpenGL context, ImGui lifecycle and the main loop.
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
     * @brief Initializes the application and starts the main loop.
     * @return Exit code — 0 on success, 1 on failure.
     */
    int run();

private:
    // --- Temporary paths — will be replaced by configuration screen ---
    static constexpr std::string_view VANILLA_PATH = R"(D:\Dev\java\oniforge\vanilla\xml)";
    static constexpr std::string_view PROJECT_PATH = R"(D:\Dev\mods\oni\TCTFagent)";

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

    // --- SDL / GL ---
    SDL_Window*   m_window   = nullptr;
    SDL_GLContext m_glContext = nullptr;
    bool          m_running  = false;

    // --- UI state ---
    int m_selectedOnccIndex = -1;
    Theme m_currentTheme = Theme::Dark;

    // Nuovo metodo privato
    void applyTheme(Theme theme);
    void renderMenuBar();


    // --- Lifecycle ---
    bool init();
    void shutdown() const;
    void mainLoop();

    // --- Render ---
    void render();
    void renderLeftPanel();
    void renderRightPanel();
    void renderGeneralTab(OniFile<ONCC::Root>& file);

    // --- Helpers ---
    [[nodiscard]] std::vector<std::string> getVanillaOncvNames() const;
    [[nodiscard]] std::vector<std::string> getVanillaTracNames() const;
};
