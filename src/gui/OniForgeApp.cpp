#include "gui/OniForgeApp.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include "fonts/ConsolasFont.hpp"

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

int OniForgeApp::run() {
    if (!init()) return 1;
    mainLoop();
    return 0;
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

bool OniForgeApp::init() {
    m_logger.separator();
    m_vanilla.loadFromFolder(std::string(VANILLA_PATH));
    m_project.loadFromFolder(std::string(PROJECT_PATH));
    snapshotOriginalPaths();
    m_logger.separator();

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
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    applyTheme(m_currentTheme);

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    loadFont(FONT_SIZES[m_currentFontIndex]);

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

        if (m_fontReloadPending) {
            loadFont(m_pendingFontSize);
            m_fontReloadPending = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render();

        ImGui::Render();
        int w, h;
        SDL_GL_GetDrawableSize(m_window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_window);
    }
}

// ---------------------------------------------------------------------------
// Font
// ---------------------------------------------------------------------------

void OniForgeApp::loadFont(const float size) {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;

    ImFontConfig config;
    config.OversampleH        = 1;
    config.OversampleV        = 1;
    config.PixelSnapH         = true;
    config.RasterizerDensity  = 1.0f;
    config.RasterizerMultiply = 1.0f;

    io.Fonts->AddFontFromMemoryCompressedTTF(
        ConsolasFont_compressed_data,
        ConsolasFont_compressed_size,
        size,
        &config
    );

    io.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

void OniForgeApp::render() {
    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##root", nullptr,
        ImGuiWindowFlags_NoTitleBar  |
        ImGuiWindowFlags_NoResize    |
        ImGuiWindowFlags_NoMove      |
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

    renderAddFileModal();
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
            if (ImGui::MenuItem("Dark",    nullptr, m_currentTheme == Theme::Dark))    { applyTheme(Theme::Dark);    m_currentTheme = Theme::Dark; }
            if (ImGui::MenuItem("Light",   nullptr, m_currentTheme == Theme::Light))   { applyTheme(Theme::Light);   m_currentTheme = Theme::Light; }
            if (ImGui::MenuItem("Classic", nullptr, m_currentTheme == Theme::Classic)) { applyTheme(Theme::Classic); m_currentTheme = Theme::Classic; }
            if (ImGui::MenuItem("Neutral", nullptr, m_currentTheme == Theme::Neutral)) { applyTheme(Theme::Neutral); m_currentTheme = Theme::Neutral; }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Font Size")) {
            for (int i = 0; i < FONT_COUNT; ++i) {
                if (ImGui::MenuItem(FONT_LABELS[i], nullptr, m_currentFontIndex == i)) {
                    m_currentFontIndex  = i;
                    m_pendingFontSize   = FONT_SIZES[i];
                    m_fontReloadPending = true;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
}

void OniForgeApp::renderLeftPanel() {
    ImGui::TextDisabled("Project");
    ImGui::Separator();

    if (const auto& onccFiles = m_project.getOnccFiles(); !onccFiles.empty()) {
        const std::string header = "ONCC (" + std::to_string(onccFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(onccFiles.size()); ++i) {
                const std::string name = onccFiles[i].path.stem().string();
                if (const bool selected = (m_selectedOnccIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOnccIndex = i;
                    m_selectedOncvIndex = -1;
                }
            }
        }
    }

    if (const auto& oncvFiles = m_project.getOncvFiles(); !oncvFiles.empty()) {
        const std::string header = "ONCV (" + std::to_string(oncvFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (int i = 0; i < static_cast<int>(oncvFiles.size()); ++i) {
                const std::string name = oncvFiles[i].path.stem().string();
                if (const bool selected = (m_selectedOncvIndex == i); ImGui::Selectable(name.c_str(), selected)) {
                    m_selectedOncvIndex = i;
                    m_selectedOnccIndex = -1;
                }
            }
        }
    }

    if (const auto& tracFiles = m_project.getTracFiles(); !tracFiles.empty()) {
        const std::string header = "TRAC (" + std::to_string(tracFiles.size()) + ")";
        if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [path, data] : tracFiles)
                ImGui::TextUnformatted(path.stem().string().c_str());
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 36.0f);
    ImGui::Separator();
    if (ImGui::Button("+ Add file..", {-1, 0})) {
        m_pendingAddFiles.clear();
        m_showAddFileModal = true;
    }
}

void OniForgeApp::renderRightPanel() {
    if (m_selectedOnccIndex >= 0 &&
        m_selectedOnccIndex < static_cast<int>(m_project.getOnccFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCC::Root>>&>(m_project.getOnccFiles());
        auto& file  = files[m_selectedOnccIndex];
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                renderGeneralTab(file);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    if (m_selectedOncvIndex >= 0 &&
        m_selectedOncvIndex < static_cast<int>(m_project.getOncvFiles().size())) {
        auto& files = const_cast<std::vector<OniFile<ONCV::Root>>&>(m_project.getOncvFiles());
        auto& file  = files[m_selectedOncvIndex];
        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("General")) {
                renderOncvTab(file);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        return;
    }

    ImGui::TextDisabled("Select a file from the project list.");
}

// ---------------------------------------------------------------------------
// ONCC General tab
// ---------------------------------------------------------------------------

void OniForgeApp::renderGeneralTab(OniFile<ONCC::Root>& file) {
    ONCC::ONCC& oncc = file.data.oncc;

    constexpr float labelWidth = 120.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

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
    if (ImGui::Button("Save##oncc", {60, 0}))
        saveWithRename(file);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Variant:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNames();
        const std::string& current = oncc.variant;
        if (ImGui::BeginCombo("##variant", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncc.variant = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

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
                if (ImGui::Selectable(name.c_str(), selected)) oncc.animations = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Health:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.health.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##health", buf, sizeof(buf))) oncc.health = buf;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Weapon Hand:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(120.0f);
    {
        char buf[64];
        strncpy(buf, oncc.weaponHand.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##weaponhand", buf, sizeof(buf))) oncc.weaponHand = buf;
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    bool hasDaodan = (oncc.hasDaodanPowers == "1");
    bool hasSuper  = (oncc.hasSupershield  == "1");
    bool cantTouch = (oncc.cantTouchThis   == "1");

    if (ImGui::Checkbox("Has Daodan Powers", &hasDaodan)) oncc.hasDaodanPowers = hasDaodan ? "1" : "0";
    if (ImGui::Checkbox("Has Supershield",   &hasSuper))  oncc.hasSupershield  = hasSuper  ? "1" : "0";
    if (ImGui::Checkbox("Can't Touch This",  &cantTouch)) oncc.cantTouchThis   = cantTouch ? "1" : "0";
}

// ---------------------------------------------------------------------------
// ONCV General tab
// ---------------------------------------------------------------------------

void OniForgeApp::renderOncvTab(OniFile<ONCV::Root>& file) {
    ONCV::Root& oncv = file.data;

    constexpr float labelWidth = 140.0f;
    const float     fieldWidth = ImGui::GetContentRegionAvail().x - labelWidth - 80.0f;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        char buf[256];
        const std::string stem = file.path.stem().string();
        strncpy(buf, stem.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##oncvname", buf, sizeof(buf)))
            file.path = file.path.parent_path() / (std::string(buf) + ".xml");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save##oncv", {60, 0}))
        saveWithRename(file);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Parent Variant:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNames();
        const std::string& current = oncv.parentVariant;
        if (ImGui::BeginCombo("##parentvariant", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncv.parentVariant = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Character Class:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNamesStripped();
        const std::string& current = oncv.characterClass;
        if (ImGui::BeginCombo("##characterclass", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncv.characterClass = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Class Hard:");
    ImGui::SameLine(labelWidth);
    ImGui::SetNextItemWidth(fieldWidth);
    {
        const auto names = getVanillaOncvNamesStripped();
        const std::string& current = oncv.characterClassHard;
        if (ImGui::BeginCombo("##characterclasshard", current.c_str())) {
            for (const auto& name : names) {
                const bool selected = (name == current);
                if (ImGui::Selectable(name.c_str(), selected)) oncv.characterClassHard = name;
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
}

// ---------------------------------------------------------------------------
// Add file modal
// ---------------------------------------------------------------------------

void OniForgeApp::renderAddFileModal() {
    if (!m_showAddFileModal) return;

    ImGui::OpenPopup("Add file from vanilla");

    const ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSize({500, 520}, ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        {io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f},
        ImGuiCond_Always, {0.5f, 0.5f}
    );

    if (!ImGui::BeginPopupModal("Add file from vanilla", &m_showAddFileModal,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        return;

    ImGui::TextDisabled("Select files to add to the project.");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("##filelist", {0, 380}, false);

    auto renderGroup = [&](const char* label, const auto& files) {
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& file : files) {
                const std::string stem = file.path.stem().string();
                bool checked = m_pendingAddFiles.contains(stem);
                const bool inProject = isInProject(stem);
                if (inProject) ImGui::BeginDisabled();
                if (ImGui::Checkbox(("##" + stem).c_str(), &checked)) {
                    if (checked) m_pendingAddFiles.insert(stem);
                    else         m_pendingAddFiles.erase(stem);
                }
                ImGui::SameLine();
                ImGui::TextUnformatted(stem.c_str());
                if (inProject) {
                    ImGui::SameLine();
                    ImGui::TextDisabled("(in project)");
                    ImGui::EndDisabled();
                }
            }
        }
    };

    renderGroup("ONCC", m_vanilla.getOnccFiles());
    renderGroup("ONCV", m_vanilla.getOncvFiles());
    renderGroup("TRAC", m_vanilla.getTracFiles());

    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Spacing();

    const std::string addLabel = m_pendingAddFiles.empty()
        ? "Add"
        : "Add (" + std::to_string(m_pendingAddFiles.size()) + ")";

    const bool canAdd = !m_pendingAddFiles.empty();
    if (!canAdd) ImGui::BeginDisabled();
    if (ImGui::Button(addLabel.c_str(), {120, 0})) {
        for (const auto& stem : m_pendingAddFiles) {
            if (!m_project.createOnccFromVanilla(stem))
                m_logger.warning("[UI] Failed to add ONCC: " + stem);
            if (!m_project.createOncvFromVanilla(stem))
                m_logger.warning("[UI] Failed to add ONCV: " + stem);
            if (!m_project.createTracFromVanilla(stem))
                m_logger.warning("[UI] Failed to add TRAC: " + stem);
        }
        snapshotOriginalPaths();
        m_pendingAddFiles.clear();
        m_showAddFileModal = false;
        ImGui::CloseCurrentPopup();
    }
    if (!canAdd) ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Cancel", {120, 0})) {
        m_pendingAddFiles.clear();
        m_showAddFileModal = false;
        ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void OniForgeApp::snapshotOriginalPaths() {
    m_originalOnccPaths.clear();
    m_originalOncvPaths.clear();
    for (int i = 0; i < static_cast<int>(m_project.getOnccFiles().size()); ++i)
        m_originalOnccPaths[i] = m_project.getOnccFiles()[i].path;
    for (int i = 0; i < static_cast<int>(m_project.getOncvFiles().size()); ++i)
        m_originalOncvPaths[i] = m_project.getOncvFiles()[i].path;
}

void OniForgeApp::saveWithRename(const OniFile<ONCC::Root>& file) {
    if (m_originalOnccPaths.contains(m_selectedOnccIndex)) {
        if (const auto& original = m_originalOnccPaths[m_selectedOnccIndex]; original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalOnccPaths[m_selectedOnccIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}

void OniForgeApp::saveWithRename(const OniFile<ONCV::Root>& file) {
    if (m_originalOncvPaths.contains(m_selectedOncvIndex)) {
        if (const auto& original = m_originalOncvPaths[m_selectedOncvIndex]; original != file.path && std::filesystem::exists(original))
            std::filesystem::remove(original);
        m_originalOncvPaths[m_selectedOncvIndex] = file.path;
    }
    m_project.saveToFolder(file.path.parent_path());
}

std::vector<std::string> OniForgeApp::getVanillaOncvNames() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getOncvFiles())
        names.push_back(path.stem().string());
    std::ranges::sort(names);
    return names;
}

std::vector<std::string> OniForgeApp::getVanillaOncvNamesStripped() const {
    std::vector<std::string> names;
    for (const auto& [path, data] : m_vanilla.getOncvFiles()) {
        std::string stem = path.stem().string();
        if (stem.starts_with("ONCV"))
            stem = stem.substr(4);
        names.push_back(stem);
    }
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

bool OniForgeApp::isInProject(const std::string& stem) const {
    for (const auto& f : m_project.getOnccFiles())
        if (f.path.stem().string() == stem) return true;
    for (const auto& f : m_project.getOncvFiles())
        if (f.path.stem().string() == stem) return true;
    for (const auto& f : m_project.getTracFiles())
        if (f.path.stem().string() == stem) return true;
    return false;
}