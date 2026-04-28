#pragma once

#include "color.h"
#include "../linear/vec.h"

template <typename T>
struct light {
    vec<T, 3> origin;
    color<T>  color;
    T         intensity;
};

using light3f = light<float>;
using light3d = light<double>;