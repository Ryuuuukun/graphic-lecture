#include "buffer.h"

bool SDLBuffer::init(SDL_Renderer* renderer) {
    buffer.reset(SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    ));
    return buffer != nullptr;
}

void SDLBuffer::destroy() {
    buffer.reset(nullptr);
}

void SDLBuffer::show(SDL_Renderer* renderer) const {
    SDL_RenderCopy(renderer, buffer.get(), nullptr, nullptr);
}

bool SDLBuffer::lock() {
    void* pixels;
    if (SDL_LockTexture(buffer.get(), nullptr, &pixels, &pitch) == 0) {
        pixels_data = static_cast<uint32_t*>(pixels);

        return true;
    }

    return false;
}

void SDLBuffer::unlock() {
    SDL_UnlockTexture(buffer.get());
}

SDLBuffer::buffer_locker SDLBuffer::get_locker() {
    return buffer_locker(this);
}

uint32_t& SDLBuffer::operator[](size_t addr) {
    return pixels_data[addr];
}

uint32_t const& SDLBuffer::operator[](size_t addr) const {
    return pixels_data[addr];
}
