#include "context.h"

void SDLContext::set_window(window_config config) {
    win_cfg = std::move(config);
}

void SDLContext::set_renderer(renderer_config config) {
    ren_cfg = std::move(config);
}

bool SDLContext::init() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        win_cfg.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        win_cfg.width, win_cfg.height,
        win_cfg.flags
    );
    if (window == nullptr) {
        SDL_Log("Failed to create window: %s", SDL_GetError());

        return false;
    }

    renderer = SDL_CreateRenderer(window, ren_cfg.index, ren_cfg.flags);
    if (renderer == nullptr) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();

        return false;
    }

    return true;
}

void SDLContext::quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}