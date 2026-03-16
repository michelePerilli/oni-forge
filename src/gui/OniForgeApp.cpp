#include "OniForgeApp.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <cstdio>

OniForgeApp::OniForgeApp() = default;

OniForgeApp::~OniForgeApp() {
    shutdown();
}

int OniForgeApp::run() {
    if (!init()) return 1;
    mainLoop();
    return 0;
}

bool OniForgeApp::init() {
    // --- SDL2 ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("[OniForgeApp] SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    // OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_window = SDL_CreateWindow(
        "OniForge",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        printf("[OniForgeApp] SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        printf("[OniForgeApp] SDL_GL_CreateContext error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1); // vsync

    // --- ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_running = true;
    return true;
}

void OniForgeApp::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_glContext) SDL_GL_DeleteContext(m_glContext);
    if (m_window)    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void OniForgeApp::mainLoop() {
    while (m_running) {
        // --- Events ---
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) m_running = false;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(m_window))
                m_running = false;
        }

        // --- ImGui frame ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render();

        // --- Rendering ---
        ImGui::Render();
        const ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window);
    }
}

void OniForgeApp::render() {
    // Fullscreen dockspace
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // Placeholder main window
    if (ImGui::Begin("OniForge")) {
        ImGui::Text("Welcome to OniForge!");
        ImGui::Separator();
        ImGui::Text("Oni mod tool — work in progress.");
    }
    ImGui::End();
}
