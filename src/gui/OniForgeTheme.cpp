#include "gui/OniForgeTheme.hpp"


const char* themeName(const Theme theme) {
    switch (theme) {
        case Theme::Dark: return "Dark";
        case Theme::Light: return "Light";
        case Theme::Classic: return "Classic";
        case Theme::Neutral: return "Neutral";
    }
    return "Unknown";
}

static void applyNeutralTheme();

void applyTheme(const Theme theme) {
    switch (theme) {
        case Theme::Dark: ImGui::StyleColorsDark();
            break;
        case Theme::Light: ImGui::StyleColorsLight();
            break;
        case Theme::Classic: ImGui::StyleColorsClassic();
            break;
        case Theme::Neutral: applyNeutralTheme();
            break;
    }
}

void applyNeutralTheme() {
    ImGui::StyleColorsDark();

    ImGuiStyle& s = ImGui::GetStyle();

    // --- Shape ---
    s.WindowRounding    = 4.0f;
    s.FrameRounding     = 3.0f;
    s.ScrollbarRounding = 3.0f;
    s.GrabRounding      = 3.0f;
    s.TabRounding       = 3.0f;
    s.ChildRounding     = 3.0f;
    s.PopupRounding     = 3.0f;

    s.WindowBorderSize = 1.0f;
    s.FrameBorderSize  = 0.0f;

    s.ItemSpacing      = ImVec2(8.0f, 5.0f);
    s.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    s.FramePadding     = ImVec2(8.0f, 4.0f);
    s.WindowPadding    = ImVec2(10.0f, 10.0f);
    s.IndentSpacing    = 18.0f;
    s.ScrollbarSize    = 12.0f;
    s.GrabMinSize      = 8.0f;

    // --- Palette ---
    // Cool grey-blue tones — dark base with subtle blue tint for readability
    const ImVec4 bg0       = ImVec4(0.13f, 0.14f, 0.17f, 1.00f); // main background
    const ImVec4 bg1       = ImVec4(0.17f, 0.18f, 0.22f, 1.00f); // child/popup background
    const ImVec4 bg2       = ImVec4(0.21f, 0.23f, 0.28f, 1.00f); // header/frame background
    const ImVec4 bg3       = ImVec4(0.26f, 0.28f, 0.34f, 1.00f); // hovered
    const ImVec4 bg4       = ImVec4(0.30f, 0.33f, 0.40f, 1.00f); // active/selected
    const ImVec4 border    = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    const ImVec4 text      = ImVec4(0.88f, 0.90f, 0.94f, 1.00f); // slightly blue-white
    const ImVec4 textDim   = ImVec4(0.50f, 0.53f, 0.60f, 1.00f);
    const ImVec4 accent    = ImVec4(0.35f, 0.40f, 0.52f, 1.00f); // blue-grey accent
    const ImVec4 accentHov = ImVec4(0.42f, 0.48f, 0.62f, 1.00f);
    const ImVec4 accentAct = ImVec4(0.30f, 0.35f, 0.46f, 1.00f);

    auto* c = s.Colors;

    c[ImGuiCol_Text]                  = text;
    c[ImGuiCol_TextDisabled]          = textDim;
    c[ImGuiCol_WindowBg]              = bg0;
    c[ImGuiCol_ChildBg]               = bg1;
    c[ImGuiCol_PopupBg]               = bg1;
    c[ImGuiCol_Border]                = border;
    c[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    c[ImGuiCol_FrameBg]               = bg2;
    c[ImGuiCol_FrameBgHovered]        = bg3;
    c[ImGuiCol_FrameBgActive]         = bg4;
    c[ImGuiCol_TitleBg]               = bg0;
    c[ImGuiCol_TitleBgActive]         = bg1;
    c[ImGuiCol_TitleBgCollapsed]      = bg0;
    c[ImGuiCol_MenuBarBg]             = bg1;
    c[ImGuiCol_ScrollbarBg]           = bg0;
    c[ImGuiCol_ScrollbarGrab]         = accent;
    c[ImGuiCol_ScrollbarGrabHovered]  = accentHov;
    c[ImGuiCol_ScrollbarGrabActive]   = accentAct;
    c[ImGuiCol_CheckMark]             = text;
    c[ImGuiCol_SliderGrab]            = accent;
    c[ImGuiCol_SliderGrabActive]      = accentAct;
    c[ImGuiCol_Button]                = bg2;
    c[ImGuiCol_ButtonHovered]         = bg3;
    c[ImGuiCol_ButtonActive]          = bg4;
    c[ImGuiCol_Header]                = bg2;
    c[ImGuiCol_HeaderHovered]         = bg3;
    c[ImGuiCol_HeaderActive]          = bg4;
    c[ImGuiCol_Separator]             = border;
    c[ImGuiCol_SeparatorHovered]      = accent;
    c[ImGuiCol_SeparatorActive]       = accentAct;
    c[ImGuiCol_ResizeGrip]            = accent;
    c[ImGuiCol_ResizeGripHovered]     = accentHov;
    c[ImGuiCol_ResizeGripActive]      = accentAct;
    c[ImGuiCol_Tab]                   = bg1;
    c[ImGuiCol_TabHovered]            = bg3;
    c[ImGuiCol_TabActive]             = bg2;
    c[ImGuiCol_TabUnfocused]          = bg0;
    c[ImGuiCol_TabUnfocusedActive]    = bg1;
    c[ImGuiCol_PlotLines]             = accent;
    c[ImGuiCol_PlotLinesHovered]      = accentHov;
    c[ImGuiCol_PlotHistogram]         = accent;
    c[ImGuiCol_PlotHistogramHovered]  = accentHov;
    c[ImGuiCol_TableHeaderBg]         = bg2;
    c[ImGuiCol_TableBorderStrong]     = border;
    c[ImGuiCol_TableBorderLight]      = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    c[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    c[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
    c[ImGuiCol_TextSelectedBg]        = ImVec4(0.50f, 0.50f, 0.50f, 0.35f);
    c[ImGuiCol_DragDropTarget]        = accentHov;
    c[ImGuiCol_NavHighlight]          = accent;
    c[ImGuiCol_NavWindowingHighlight] = accentHov;
    c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
}
