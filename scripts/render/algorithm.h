#pragma once

#include <memory>

#include "../linear/ray.h"
#include "../component/color.h"
#include "../component/light.h"
#include "../scene/scene.h"

struct NothingTracing {
    color3f operator()(ray3f const& ray) const {
        return { 255 };
    }
};

constexpr float VISIBLIE_DISTANCE = 10e7;

struct SimpleTracing {
    std::shared_ptr<Scene> scene;

    RaycastHit raycast;

    color3f background;
    color3f hit;

public:
    explicit SimpleTracing(color3f background, color3f hit)
        : background(background), hit(hit)
    {}

    void with(std::shared_ptr<Scene> _scene) {
        scene = _scene;
    }

    color3f operator()(ray3f const& ray) {
        raycast.reset();
        for (auto const& model : scene->models) {
            model.trace(ray, raycast);
        }

        return raycast.param < VISIBLIE_DISTANCE ? hit : background;
    }
};


struct LightTracing {
    std::shared_ptr<Scene> scene;

    light3f direct;
    light3f ambience;
    light3f background;

    RaycastHit raycast;

public:
    explicit LightTracing(light3f direct, light3f ambience, light3f background)
        : direct(direct), ambience(ambience), background(background)
    {}

    void with(std::shared_ptr<Scene> _scene) {
        scene = _scene;
    }

    color3f operator()(ray3f const& ray) {
        raycast.reset();
        for (auto const& model : scene->models) {
            model.trace(ray, raycast);
        }

        color3f result = background.color * background.intensity;
        if (raycast.param < VISIBLIE_DISTANCE) {
            auto& attibutes = raycast.get();

            color3f coeff(0.9f, 0.3f, 0.7f);
            if (attibutes.material.has()) {
                coeff = attibutes.material.get().coeff;
            }

            result = ambience.color * ambience.intensity * coeff;

            float cos = dot(normalized(raycast.point - direct.origin), attibutes.normal.get());
            if (cos > 0) {
                result += direct.color * direct.intensity * cos * coeff;
            }
        }

        return clamp(result, 0.0f, 255.0f);
    }
};

struct ShadowLightTracing {
    std::shared_ptr<Scene> scene;

    light3f direct;
    light3f ambience;
    light3f background;

    RaycastHit raycast_light;
    RaycastHit raycast_shadow;

public:
    explicit ShadowLightTracing(light3f direct, light3f ambience, light3f background)
        : direct(direct), ambience(ambience), background(background)
    {}

    void with(std::shared_ptr<Scene> _scene) {
        scene = _scene;
    }

    color3f operator()(ray3f const& ray) {
        raycast_light.reset();
        for (auto const& model : scene->models) {
            model.trace(ray, raycast_light);
        }

        color3f result = background.color * background.intensity;
        if (raycast_light.param < VISIBLIE_DISTANCE) {
            auto& attibutes = raycast_light.get();

            result = ambience.color * ambience.intensity;

            ray3f shadow_ray(raycast_light.point, direct.origin);

            bool shadow = false;
            raycast_shadow.reset();
            for (auto const& model : scene->models) {
                model.trace(shadow_ray, raycast_shadow);
                if (raycast_shadow.param > 10e-2 && raycast_shadow.param < VISIBLIE_DISTANCE) {
                    shadow = true;
                }
            }

            if (!shadow) {
                float cos = dot(normalized(raycast_light.point - direct.origin), attibutes.normal.get());
                if (cos > 0) {
                    result += direct.color * direct.intensity * cos;
                }
            }
        }

        return clamp(result, 0.0f, 255.0f);
    }
};