#pragma once

#include "../component/color.h"
#include "../utility/buffer.h"

struct VignetteEffect {
    static constexpr float INTENSITY = 0.4f;

    void operator()(SDLBuffer& buffer) const {
        auto locker = buffer.get_locker();
        if (locker.lock()) {
            size_t width  = buffer.width;
            size_t height = buffer.height;

            float middle_x = width / 2;
            float middle_y = height / 2;

            for (size_t y = 0; y < buffer.height; ++y) {
                for (size_t x = 0; x < buffer.width; ++x) {
                    float offset_x = x - middle_x;
                    float offset_y = y - middle_y;

                    float factor = 1.0f - INTENSITY * static_cast<float>(SDL_sqrt(offset_x * offset_x + offset_y * offset_y)) / height;

                    buffer[y * buffer.width + x] = (color<float>::from_argb(buffer[y * buffer.width + x]) * factor).to_argb();
                }
            }
        }
    }
};
