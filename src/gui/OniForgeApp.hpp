// OniForgeApp.hpp
#pragma once

// Forward declarations — SDL types
struct SDL_Window;
typedef void* SDL_GLContext;

/**
 * @brief Main application class for OniForge.
 */
class OniForgeApp {
public:
    OniForgeApp();
    ~OniForgeApp();

    OniForgeApp(const OniForgeApp&) = delete;
    OniForgeApp& operator=(const OniForgeApp&) = delete;
    OniForgeApp(OniForgeApp&&) = delete;
    OniForgeApp& operator=(OniForgeApp&&) = delete;

    int run();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_GLContext m_glContext = nullptr;
    bool          m_running  = false;

    bool init();
    void shutdown();
    void mainLoop();
    void render();
};