#include <SDL.h>

#include "linear/transform.h"
#include "postprocess/fxaa.h"
#include "postprocess/vignette.h"

#include "render/algorithm.h"
#include "render/tracing.h"
#include "scene/scene.h"
#include "utility/buffer.h"
#include "utility/context.h"

constexpr size_t WIDTH  = 1200;
constexpr size_t HEIGHT = WIDTH / 16 * 9;

int main(int argc, char* argv[]) {
    SDLContext context;

    context.set_window({
        .title  = "Ray tracing",
        .width  = WIDTH,
        .height = HEIGHT,
        .flags  = SDL_WINDOW_SHOWN
    });
    context.set_renderer({
        .index = -1,
        .flags = SDL_RENDERER_ACCELERATED
    });

    if (!context.init()) {
        return 1;
    }

    SDLBuffer buffer(WIDTH, HEIGHT);
    if (!buffer.init(context.renderer)) {
        return 1;
    }

    auto scene = std::make_shared<Scene>();
    scene->load_models({
        {
            .filename  = "../resources/icosphere.obj",
            .transform = fin::rotate_y(-std::numbers::pi / 4),
            .material  = std::make_shared<Material>(color3f(0.7f))
        },
        {
            .filename  = "../resources/torus.obj",
            .transform = fin::translate(-1.0f, 0.0f, 0.0f),
            .material  = std::make_shared<Material>(color3f(0.2f, 0.6f, 0.9f))
        }
    });

    auto camera = std::make_shared<Camera>(800.0f, vec3f{ 2, 2, 3 });
    camera->look_at({ 0, 0, 0 });

    // auto algorithm = std::make_shared<SimpleTracing>(color3f{ 32.0f }, color3f{ 255.0f });
    auto algorithm = std::make_shared<LightTracing>(
        light3f{ .origin = { -5.0f, -5.0f, -5.0f }, .color = { 1.0f  }, .intensity = 255.0f }, // direct
        light3f{ .origin = {  0.0f,  0.0f,  0.0f }, .color = { 0.5f  }, .intensity = 72.0f  }, // ambience
        light3f{ .origin = {  0.0f,  0.0f,  0.0f }, .color = { 1.0f  }, .intensity = 200.0f }  // background
    );
    algorithm->with(scene);

    TracingDevice device(1.0f);
    device.trace(buffer, *camera, *algorithm);

    FXAntiAliasing{ }(buffer);
    VignetteEffect{ }(buffer);

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;

                break;
            }
        }

        SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
        SDL_RenderClear(context.renderer);
        buffer.show(context.renderer);
        SDL_RenderPresent(context.renderer);

        SDL_Delay(30);
    }

    return 0;
}
