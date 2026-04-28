#pragma once

#include "color.h"

struct Material {
    color3f coeff;

    Material() = default;

    Material(color3f coeff)
        : coeff(coeff)
    {}
};
