#include "gui/OniForgeApp.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <algorithm>
#include <cstdio>

// ---------------------------------------------------------------------------
// Constructor / Destructor
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
    , m_project(m_repos, m_vanilla, m_logger) {}

OniForgeApp::~OniForgeApp() {
    shutdown();
}

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
    // Load data
    m_logger.separator();
    m_vanilla.loadFromFolder(std::string(VANILLA_PATH));
    m_project.loadFromFolder(std::string(PROJECT_PATH));
    m_logger.separator();

    // SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("[OniForgeApp] SDL_Init error: %s\n", SDL_GetError());
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
        "OniForge",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
    SDL_GL_SetSwapInterval(1);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_running = true;
    return true;
}

void OniForgeApp::shutdown() const {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (m_glContext) SDL_GL_DeleteContext(m_glContext);
    if (m_window)    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void OniForgeApp::mainLoop() {
    while (m_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) m_running = false;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(m_window))
                m_running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render();

        ImGui::Render();
        const ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window);
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
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_MenuBar
    );

    renderMenuBar();

    constexpr float leftWidth = 250.0f;
    const float rightWidth = ImGui::GetContentRegionAvail().x - leftWidth - 8.0f;

    // Left panel
    ImGui::BeginChild("##left", {leftWidth, 0}, true);
    renderLeftPanel();
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel
    ImGui::BeginChild("##right", {rightWidth, 0}, true);
    renderRightPanel();
    ImGui::EndChild();

    ImGui::End();
}

void OniForgeApp::renderLeftPanel() {
    ImGui::TextDisabled("Project");
    ImGui::Separator();

    // ONCC group
    if (const auto& onccFiles = m_project.getOnccFiles(); !onccFiles.empty()) {
        const std::string header = "ONCC (" + std::to_string(onccFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(onccFiles.size()); ++i) {
                const std::string name = onccFiles[i].path.stem().string();
                if (const bool selected = (m_selectedOnccIndex == i); ImGui::Selectable(name.c_str(), selected))
                    m_selectedOnccIndex = i;
            }
        }
    }

    // ONCV group (read-only for now)
    if (const auto& oncvFiles = m_project.getOncvFiles(); !oncvFiles.empty()) {
        const std::string header = "ONCV (" + std::to_string(oncvFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [path, data] : oncvFiles)
                ImGui::TextUnformatted(path.stem().string().c_str());
        }
    }

    // TRAC group (read-only for now)
    if (const auto& tracFiles = m_project.getTracFiles(); !tracFiles.empty()) {
        const std::string header = "TRAC (" + std::to_string(tracFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [path, data] : tracFiles)
                ImGui::TextUnformatted(path.stem().string().c_str());
        }
    }

    // Add file button at the bottom
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 36.0f);
    ImGui::Separator();
    if (ImGui::Button("+ Add file..", {-1, 0}))
        m_logger.info("[UI] Add file clicked — not yet implemented.");
}

void OniForgeApp::renderRightPanel() {
    if (m_selectedOnccIndex < 0 ||
        m_selectedOnccIndex >= static_cast<int>(m_project.getOnccFiles().size())) {
        ImGui::TextDisabled("Select a file from the project list.");
        return;
    }

    // Get mutable reference
    auto& files = const_cast<std::vector<OniFile<ONCC::Root>>&>(m_project.getOnccFiles());
    auto& file  = files[m_selectedOnccIndex];

    if (ImGui::BeginTabBar("##tabs")) {
        if (ImGui::BeginTabItem("General")) {
            renderGeneralTab(file);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void OniForgeApp::renderGeneralTab(OniFile<ONCC::Root>& file) {
    ONCC::ONCC& oncc = file.data.oncc;

    constexpr float labelWidth = 120.0f;
    const float fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

    // Name
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        char buf[256];
        const std::string stem = file.path.stem().string();
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##name", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }

    ImGui::SameLine();
    if (ImGui::Button("Save", {60, 0}))
        m_project.saveToFolder(file.path.parent_path());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Variant (ONCV combobox)
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Variant:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNames();
        const std::string& current = oncc.variant.value_or("");
        if (ImGui::BeginCombo("##variant", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected))
                    oncc.variant = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    // Animations (TRAC combobox)
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Animations:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaTracNames();
        const std::string& current = oncc.animations;
        if (ImGui::BeginCombo("##animations", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected))
                    oncc.animations = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Health
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Health:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.health.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##health", buf, sizeof(buf)))
            oncc.health = buf;
    }

    // WeaponHand
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Weapon Hand:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.weaponHand.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##weaponhand", buf, sizeof(buf)))
            oncc.weaponHand = buf;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Flags (checkboxes)
    {
        bool hasDaodan   = (oncc.hasDaodanPowers == "1");
        bool hasSuper    = (oncc.hasSupershield  == "1");
        bool cantTouch   = (oncc.cantTouchThis   == "1");

        if (ImGui::Checkbox("Has Daodan Powers", &hasDaodan))
            oncc.hasDaodanPowers = hasDaodan ? "1" : "0";
        if (ImGui::Checkbox("Has Supershield",   &hasSuper))
            oncc.hasSupershield  = hasSuper  ? "1" : "0";
        if (ImGui::Checkbox("Can't Touch This",  &cantTouch))
            oncc.cantTouchThis   = cantTouch ? "1" : "0";
    }
}

void OniForgeApp::applyTheme(const Theme theme) {
    switch (theme) {
        case Theme::Dark:    ImGui::StyleColorsDark();    break;
        case Theme::Light:   ImGui::StyleColorsLight();   break;
        case Theme::Classic: ImGui::StyleColorsClassic(); break;
    }
    m_currentTheme = theme;
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
            if (ImGui::MenuItem("Dark",    nullptr, m_currentTheme == Theme::Dark))
                applyTheme(Theme::Dark);
            if (ImGui::MenuItem("Light",   nullptr, m_currentTheme == Theme::Light))
                applyTheme(Theme::Light);
            if (ImGui::MenuItem("Classic", nullptr, m_currentTheme == Theme::Classic))
                applyTheme(Theme::Classic);
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}


// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

std::vector<std::string> OniForgeApp::getVanillaOncvNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getOncvFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}

std::vector<std::string> OniForgeApp::getVanillaTracNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getTracFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}
