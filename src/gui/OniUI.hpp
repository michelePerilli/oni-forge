#pragma once
#include <imgui.h>
#include "model/OniFile.hpp"

namespace OniUI {


    namespace Colors {
        inline constexpr ImVec4 unmodified = { 0.80f, 0.80f, 0.80f, 1.0f };
        inline constexpr ImVec4 modified   = { 0.40f, 0.61f, 0.85f, 1.0f };
        inline constexpr ImVec4 created    = { 0.40f, 0.70f, 0.40f, 1.0f };
        inline constexpr ImVec4 deleted    = { 0.50f, 0.50f, 0.50f, 1.0f };
    }

    /**
     * @brief Pushes the color associated with the given file status to the ImGui style stack.
     * @param status The current status of the file.
     */
    inline void PushFileStatusColor(FileStatus status) {
        switch (status) {
            case FileStatus::Modified: ImGui::PushStyleColor(ImGuiCol_Text, Colors::modified); break;
            case FileStatus::Created:  ImGui::PushStyleColor(ImGuiCol_Text, Colors::created); break;
            case FileStatus::Deleted:  ImGui::PushStyleColor(ImGuiCol_Text, Colors::deleted); break;
            default:                   ImGui::PushStyleColor(ImGuiCol_Text, Colors::unmodified); break;
        }
    }

    /**
     * @brief Pops the file status color from the ImGui style stack.
     * @param count The number of colors to pop. Default is 1.
     */
    inline void PopFileStatusColor(int count = 1) {
        ImGui::PopStyleColor(count);
    }
}