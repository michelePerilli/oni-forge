#pragma once

#include "model/Config.hpp"
#include "gui/OniGuiRenderer.hpp"
#include <functional>

namespace OniForge {

/**
 * @brief Modal dialog for editing application settings and configuration.
 */
class SettingsModal {
public:
    SettingsModal(Config& config, OniGuiRenderer& renderer);
    ~SettingsModal() = default;

    /**
     * @brief Opens the modal dialog.
     */
    void open();

    /**
     * @brief Renders the modal. Should be called every frame.
     *
     * @param onSave Callback triggered when user clicks "Save".
     */
    void render(const std::function<void()>& onSave);

private:
    Config&         m_config;
    OniGuiRenderer& m_renderer;

    bool m_isOpen           = false;
    bool m_fontChanged      = false;
    bool m_themeChanged     = false;
    int  m_selectedFontIndex = -1;
    bool  m_fontReloadPending = false;
    float m_pendingFontSize   = 15.0f;
    void updateFontIndex();
};

} // namespace OniForge
