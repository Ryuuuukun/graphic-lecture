#pragma once

#include <SDL.h>
#include <string>
#include <cstdint>

struct SDLContext {
    struct window_config {
        std::string title;
        size_t      width;
        size_t      height;
        uint32_t    flags;
    };

    struct renderer_config {
        int32_t  index;
        uint32_t flags;
    };

    window_config   win_cfg;
    renderer_config ren_cfg;

    SDL_Renderer* renderer;
    SDL_Window*   window;

public:
    explicit SDLContext() = default;

    void set_window(window_config config);
    void set_renderer(renderer_config config);

    bool init();
    void quit();

    ~SDLContext() {
        quit();
    }
};