#pragma once

#include "gui/OniForgeTheme.hpp"

struct SDL_Window;
typedef void* SDL_GLContext;

/**
 * @brief Manages the SDL2 + OpenGL + ImGui lifecycle.
 *
 * Responsible for window creation, OpenGL context, ImGui initialization,
 * font loading, frame begin/end and shutdown.
 * OniForgeApp owns this and calls it without knowing anything about SDL or OpenGL.
 */
class OniGuiRenderer {
public:
    OniGuiRenderer() = default;
    ~OniGuiRenderer();

    OniGuiRenderer(const OniGuiRenderer&) = delete;
    OniGuiRenderer& operator=(const OniGuiRenderer&) = delete;
    OniGuiRenderer(OniGuiRenderer&&) = delete;
    OniGuiRenderer& operator=(OniGuiRenderer&&) = delete;

    /**
     * @brief Initializes SDL2, OpenGL context, ImGui and loads the default font.
     * @return True on success, false on any failure.
     */
    [[nodiscard]] bool init(const char* windowTitle, int width, int height,
                            Theme theme, float fontSize);

    /**
     * @brief Shuts down ImGui, SDL2 and destroys the OpenGL context.
     */
    void shutdown();

    /**
     * @brief Polls SDL events and begins a new ImGui frame.
     * @param running Set to false if the window close event is received.
     */
    void beginFrame(bool& running);

    /**
     * @brief Renders the ImGui draw data and swaps the SDL2 buffer.
     */
    void endFrame();

    /**
     * @brief Reloads the font atlas with the given size.
     * Must be called outside of a frame (before beginFrame).
     */
    void loadFont(float size);

    /**
     * @brief Applies a theme to the current ImGui style.
     */
    void applyTheme(Theme theme) const;

    [[nodiscard]] bool isInitialized() const { return m_initialized; }

private:
    SDL_Window*   m_window      = nullptr;
    SDL_GLContext m_glContext    = nullptr;
    bool          m_initialized = false;
};
