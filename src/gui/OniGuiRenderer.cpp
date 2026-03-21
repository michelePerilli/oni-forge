#include "gui/OniGuiRenderer.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#include "misc/freetype/imgui_freetype.h"
#include "fonts/InterMediumFont.hpp"
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------

OniGuiRenderer::~OniGuiRenderer() {
    shutdown();
}

// ---------------------------------------------------------------------------
// Init
// ---------------------------------------------------------------------------

bool OniGuiRenderer::init(const char* windowTitle, const int width, const int height,
                          const Theme theme, const float     fontSize) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("[ImGuiRenderer] SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow(
        windowTitle,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        printf("[ImGuiRenderer] SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        printf("[ImGuiRenderer] SDL_GL_CreateContext error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    applyTheme(theme);

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    loadFont(fontSize);

    m_initialized = true;
    return true;
}

// ---------------------------------------------------------------------------
// Shutdown
// ---------------------------------------------------------------------------

void OniGuiRenderer::shutdown() {
    if (!m_initialized) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (m_glContext) SDL_GL_DeleteContext(m_glContext);
    if (m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
    m_initialized = false;
}

// ---------------------------------------------------------------------------
// Frame
// ---------------------------------------------------------------------------

void OniGuiRenderer::beginFrame(bool& running) const {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(m_window))
            running = false;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void OniGuiRenderer::endFrame() const {
    ImGui::Render();
    int w, h;
    SDL_GL_GetDrawableSize(m_window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);
}

// ---------------------------------------------------------------------------
// Font
// ---------------------------------------------------------------------------

void OniGuiRenderer::loadFont(const float size) {
    const ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->Flags |= ImGuiFreeTypeBuilderFlags_ForceAutoHint;

    ImFontConfig config;
    config.OversampleH        = 1;
    config.OversampleV        = 1;
    config.PixelSnapH         = true;
    config.RasterizerDensity  = 1.0f;
    config.RasterizerMultiply = 1.0f;

    io.Fonts->AddFontFromMemoryCompressedTTF(
        InterMediumFont_compressed_data,
        InterMediumFont_compressed_size,
        size,
        &config
    );

    io.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

// ---------------------------------------------------------------------------
// Theme
// ---------------------------------------------------------------------------

void OniGuiRenderer::applyTheme(const Theme theme) {
    ::applyTheme(theme);
}
