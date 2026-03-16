#include <SDL.h>
#include "gui/OniForgeApp.hpp"

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    OniForgeApp app;
    return app.run();
}