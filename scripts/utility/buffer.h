#pragma once

#include <SDL_render.h>
#include <cstdint>
#include <memory>

struct SDLBuffer {
    struct sdl_texture_destroy {
        void operator()(SDL_Texture* texture) const {
            SDL_DestroyTexture(texture);
        }
    };

    struct buffer_locker {
        SDLBuffer* buffer;

    public:
        buffer_locker(SDLBuffer* buffer)
            : buffer(buffer)
        {}

        bool lock() {
            return buffer->lock();
        }

        void unlock() {
            return buffer->unlock();
        }

        ~buffer_locker() {
            buffer->unlock();
        }
    };

    uint32_t* pixels_data;
    int       pitch;

    const size_t width;
    const size_t height;
    std::unique_ptr<SDL_Texture, sdl_texture_destroy> buffer;

public:
    explicit SDLBuffer(size_t width, size_t height)
        : pixels_data(nullptr), pitch(0), width(width), height(height)
    {}

    bool init(SDL_Renderer* renderer);
    void destroy();

    void show(SDL_Renderer* renderer) const;

    bool lock();
    void unlock();

    buffer_locker get_locker();

    uint32_t& operator[](size_t address);
    uint32_t const& operator[](size_t address) const;
};
