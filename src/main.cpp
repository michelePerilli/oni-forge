/**
 * @file main.cpp
 * @brief Main entry point for the OniForge application.
 *
 * Initializes SDL and runs the main application loop.
 */

#include <SDL.h>
#include "gui/OniForgeApp.hpp"

int main() {
    SDL_SetMainReady();
    OniForgeApp app;
    return app.run();
}
