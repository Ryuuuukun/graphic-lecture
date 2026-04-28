#pragma once

#include "../linear/vec.h"
#include "../linear/transform.h"

struct Camera {
    float focal;
    vec3f origin;

    vec3f axis_x;
    vec3f axis_y;
    vec3f axis_z;

public:
    explicit Camera(float focal, vec3f origin)
        : focal(focal)
        , origin(origin)
        , axis_x(1, 0, 0)
        , axis_y(0, 1, 0)
        , axis_z(0, 0, -1)
    {}

    void look_at(vec3f point);

    void rotate(affine3f const& rotate);
    void translate(affine3f const& translate);
};
