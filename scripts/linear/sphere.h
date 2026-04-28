#pragma once

#include "qut.h"
#include "ray.h"
#include "vec.h"

// It is assumed that the dimension does not exceed 3 and is greater than 1,
// since the algorithms incorrectly extend to dimensions above 3.

template <typename T, size_t DIM>
struct hypersphere {
    vec<T, DIM> origin;
    T           radius;

public:
    hypersphere() = default;

    hypersphere(vec<T, DIM> origin, T radius)
        : origin(origin), radius(radius)
    {}

    static hypersphere bound_sphere(std::vector<vec<T, DIM>> const& points) {
        vec<T, DIM> _min = {  std::numeric_limits<T>::infinity() };
        vec<T, DIM> _max = { -std::numeric_limits<T>::infinity() };
        for (auto const& vert : points) {
            _min = min(_min, vert);
            _max = max(_max, vert);
        }

        return { (_max + _min) / 2, norm((_max - _min) / 2) };
    }

    bool intersect(ray<T, DIM> const& ray) const {
        auto delta = get_delta(ray);
        return delta.y * delta.y >= delta.x * delta.z;
    }

    bool hits(vec<T, DIM> const& vec) const {
        return sqr(vec - origin) <= radius * radius;
    }

    vec<T, 3> get_delta(ray<T, DIM> const& ray) const {
        auto offset = ray.origin - origin;
        return {
            sqr(ray.delta),
            dot(ray.delta, offset),
            sqr(offset) - radius * radius
        };
    }
};

template <typename T>
using circle = hypersphere<T, 2>;

template <typename T>
using sphere = hypersphere<T, 3>;

using sphere3f = sphere<float>;
using sphere3d = sphere<double>;
