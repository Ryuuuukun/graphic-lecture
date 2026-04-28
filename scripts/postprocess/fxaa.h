#pragma once

#include <vector>

#include "../component/color.h"
#include "../utility/benchmark.h"
#include "../utility/buffer.h"

struct FXAntiAliasing {
    static constexpr float EDGE_THRESHOLD_MIN = 0.012f;
    static constexpr float EDGE_THRESHOLD_MAX = 0.125f;
    static constexpr float SUBPIX_STRENGTH    = 0.9f;

    void operator()(SDLBuffer& buffer) const {
        auto locker = buffer.get_locker();
        if (locker.lock()) {
            Benchmark benchmark("fx anti aliasing");

            size_t width  = buffer.width;
            size_t height = buffer.height;

            std::vector<uint32_t> result(width * height);

            for (size_t y = 1; y < height - 1; ++y) {
                for (size_t x = 1; x < width - 1; ++x) {
                    float lumaM = luma<float>(buffer[y * width + x]);
                    float lumaW = luma<float>(buffer[y * width + x - 1]);
                    float lumaE = luma<float>(buffer[y * width + x + 1]);
                    float lumaN = luma<float>(buffer[(y - 1) * width + x]);
                    float lumaS = luma<float>(buffer[(y + 1) * width + x]);

                    float low_luma  = std::min({ lumaM, lumaW, lumaE, lumaN, lumaS });
                    float high_luma = std::max({ lumaM, lumaW, lumaE, lumaN, lumaS });

                    if (high_luma - low_luma < std::max(EDGE_THRESHOLD_MIN, high_luma * EDGE_THRESHOLD_MAX)) {
                        result[y * width + x] = buffer[y * width + x];
                        continue;
                    }

                    float lumaNW = luma<float>(buffer[(y - 1) * width + (x - 1)]);
                    float lumaNE = luma<float>(buffer[(y - 1) * width + (x + 1)]);
                    float lumaSW = luma<float>(buffer[(y + 1) * width + (x - 1)]);
                    float lumaSE = luma<float>(buffer[(y + 1) * width + (x + 1)]);

                    float edge_vert =
                        std::abs((0.25f * lumaNW) + (-0.5f * lumaN) + (0.25f * lumaNE)) +
                        std::abs((0.50f * lumaW ) + (-1.0f * lumaM) + (0.50f * lumaE )) +
                        std::abs((0.25f * lumaSW) + (-0.5f * lumaS) + (0.25f * lumaSE));

                    float edge_horz =
                        std::abs((0.25f * lumaNW) + (-0.5f * lumaW) + (0.25f * lumaSW)) +
                        std::abs((0.50f * lumaN ) + (-1.0f * lumaM) + (0.50f * lumaS )) +
                        std::abs((0.25f * lumaNE) + (-0.5f * lumaE) + (0.25f * lumaSE));

                    bool is_horz = edge_horz >= edge_vert;

                    float l1 = is_horz ? lumaN : lumaW;
                    float l2 = is_horz ? lumaS : lumaE;

                    float grad1 = l1 - lumaM;
                    float grad2 = l2 - lumaM;

                    float luma_avg = (2.0f * (lumaN + lumaS + lumaW + lumaE) + lumaNW + lumaNE + lumaSW + lumaSE) / 12.0f;
                    float sub_pixel_offset = std::clamp(std::abs(luma_avg - lumaM) / (high_luma - low_luma), 0.0f, 1.0f);

                    float blend_factor = sub_pixel_offset * sub_pixel_offset * SUBPIX_STRENGTH;


                    uint32_t neighbor;
                    bool is_steeper = std::abs(grad1) >= std::abs(grad2);
                    if (is_horz) {
                        neighbor = is_steeper ? buffer[(y - 1) * width + x] : buffer[(y + 1) * width + x];
                    } else {
                        neighbor = is_steeper ? buffer[y * width + (x - 1)] : buffer[y * width + (x + 1)];
                    }

                    result[y * width + x] = lerp(color3f::from_argb(buffer[y * width + x]), color3f::from_argb(neighbor), blend_factor).to_argb();
                }
            }

            for (size_t y = 1; y < height - 1; ++y) {
                for (size_t x = 1; x < width - 1; ++x) {
                    buffer[y * width + x] = result[y * width + x];
                }
            }
        }
    }
};