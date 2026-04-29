#pragma once

#include <cmath>

#include "qut.h"
#include "ray.h"
#include "vec.h"

// It is assumed that the dimension does not exceed 3 and is greater than 1,
// since the algorithms incorrectly extend to dimensions above 3.

template <typename T, size_t DIM>
struct hyperbox {
    static constexpr T EPS = T(10e-5);

    vec<T, DIM> min;
    vec<T, DIM> max;

public:
    hyperbox() = default;

    hyperbox(vec<T, DIM> min, vec<T, DIM> max)
        : min(min), max(max)
    {}

    static hyperbox from_bound(std::vector<vec<T, DIM>> const& points) {
        vec<T, DIM> _min = {  std::numeric_limits<T>::infinity() };
        vec<T, DIM> _max = { -std::numeric_limits<T>::infinity() };
        for (auto const& vert : points) {
            _min = ::min(_min, vert);
            _max = ::max(_max, vert);
        }

        return { _min - EPS, _max + EPS };
    }

    bool intersect(ray<T, DIM> const& ray) const {
        auto params = get_params(ray);
        return params.y > std::max(params.x, T(0));
    }

    bool intersect(hyperbox const& box) const {
        auto this_origin = T(0.5) * (max + min);
        auto origin = T(0.5) * (box.max + box.min);

        bool result = true;
        for (size_t i = 0; i < DIM; ++i) {
            result &=
                std::abs(this_origin[i] - origin[i]) <= (max[i] - min[i]) * T(0.5) + (box.max[i] - box.min[i]) * T(0.5);
        }

        return result;
    }

    bool hits(vec<T, DIM> const& vec) const {
        return sqr(vec - (max + min) * T(0.5)) <= sqr((max - min) * T(0.5));
    }

    bool hits(vec<T, DIM> const& a, vec<T, DIM> const& b, vec<T, DIM> const& c) const {
        if (hits(a) || hits(b) || hits(c)) {
            return true;
        }

        return intersect(hyperbox{ ::min(a, ::min(b, c)), ::max(a, ::max(b, c)) });
    }

    vec<T, 2> get_params(ray<T, DIM> const& ray) const {
        vec<T, 2> params = {
            -std::numeric_limits<T>::infinity(),
             std::numeric_limits<T>::infinity()
        };

        for (size_t i = 0; i < DIM; ++i) {
            T t1 = (min[i] - ray.origin[i]) * ray.inv_delta[i];
            T t2 = (max[i] - ray.origin[i]) * ray.inv_delta[i];

            params.x = std::max(params.x, std::min(t1, t2));
            params.y = std::min(params.y, std::max(t1, t2));
        }

        return params;
    }
};

template <typename T>
using square = hyperbox<T, 2>;

template <typename T>
using bbox = hyperbox<T, 3>;

using bbox3f = bbox<float>;
using bbox3d = bbox<double>;

template <typename T, size_t DIM>
auto origin(hyperbox<T, DIM> const& box) {
    return T(0.5) * (box.max + box.min);
}

template <typename T, size_t DIM>
auto size(hyperbox<T, DIM> const& box) {
    return T(0.5) * (box.max - box.min);
}