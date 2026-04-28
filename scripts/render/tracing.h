#pragma once

#include "../linear/vec.h"
#include "../linear/ray.h"
#include "../scene/camera.h"
#include "../utility/buffer.h"
#include "../utility/benchmark.h"

struct TracingDevice {
    float scale;

public:
    explicit TracingDevice(float scale)
        : scale(scale)
    {}

    template <typename Algorithm>
    void trace(SDLBuffer& buffer, Camera const& camera, Algorithm&& algorithm) const {
        auto locker = buffer.get_locker();
        if (locker.lock()) {
            Benchmark benchmark("tracing device");

            vec3f step_x = camera.axis_x / scale;
            vec3f step_y = camera.axis_y / scale;

            ray3f ray(camera.origin, camera.axis_z * camera.focal -
                                        (buffer.width  / 2.0f - 0.5f) * step_x +
                                        (buffer.height / 2.0f - 0.5f) * step_y);

            size_t addr = 0;
            for (size_t y = 0; y < buffer.height; ++y) {
                for (size_t x = 0; x < buffer.width; ++x) {
                    ray.delta += step_x;
                    ray.inv_delta = {
                        1 / ray.delta.x,
                        1 / ray.delta.y,
                        1 / ray.delta.z
                    };

                    buffer[addr++] = algorithm(ray).to_argb();
                }
                ray.delta -= step_y;
                ray.delta -= step_x * buffer.width;
            }
        }
    }
};
