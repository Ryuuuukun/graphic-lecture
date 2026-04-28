#pragma once

#include <functional>
#include <variant>

#include "../linear/vec.h"
#include "../component/material.h"

struct RaycastHit {
    template <typename... Types>
    struct overload : Types... { using Types::operator()...; };

    template <typename T>
    struct data_handler {
        std::variant<T, T const*> handle;

    public:
        data_handler()
            : handle(nullptr)
        {}

        data_handler& operator=(T const* data) {
            handle = data;
            return *this;
        }

        data_handler& operator=(T data) {
            handle = std::move(data);
            return *this;
        }

        bool has() const {
            return std::visit(overload {
                [](T const* data) { return data != nullptr; },
                [](T const& data) { return true;            }
            }, handle);
        }

        T const& get() const {
            return std::visit(overload {
                [](T const* data) -> T const& { return *data; },
                [](T const& data) -> T const& { return  data; }
            }, handle);
        }
    };

    struct raycast_storage {
        data_handler<vec3f>    normal;
        data_handler<vec3f>    origin;
        data_handler<Material> material;

        void reset() {
            normal = nullptr;
            origin = nullptr;
        }
    };

    float param;
    vec3f point;

    raycast_storage attributes;
    std::function<void (raycast_storage&)> config;

public:
    explicit RaycastHit()
        : param(std::numeric_limits<float>::infinity())
    {}

    void send(float _param, vec3f _point, std::function<void (raycast_storage&)> _config) {
        if (_param < param) {
            param  = _param;
            point  = _point;
            config = std::move(_config);
        }
    }

    void send(float _param, vec3f _point) {
        send(_param, _point, nullptr);
    }

    void reset() {
        param  = std::numeric_limits<float>::infinity();
        config = nullptr;
        attributes.reset();
    }

    raycast_storage& get() {
        attributes.reset();
        if (config != nullptr) {
            config(attributes);
        }

        return attributes;
    }
};
