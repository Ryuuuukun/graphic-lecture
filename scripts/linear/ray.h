#pragma once

#include "vec.h"

template <typename T, size_t DIM>
struct ray {
    vec<T, DIM> origin;
    vec<T, DIM> delta;
    vec<T, DIM> inv_delta;

public:
    ray() = default;

    ray(vec<T, DIM> origin, vec<T, DIM> delta)
        : origin(origin), delta(delta)
    {}

    template <typename V> requires std::is_convertible_v<V, T>
    ray(vec<V, DIM> origin, vec<V, DIM> delta)
        : origin(origin), delta(delta)
    {}
};

using ray3f = ray<float, 3>;
using ray3d = ray<double, 3>;
