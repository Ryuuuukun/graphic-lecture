#pragma once

#include "../linear/vec.h"
#include "../linear/transform.h"

struct Camera {
    float focal;
    vec3f origin;

    vec3f axis_x = {  1.0f,  0.0f,  0.0f };
    vec3f axis_y = {  0.0f,  1.0f,  0.0f };
    vec3f axis_z = {  0.0f,  0.0f, -1.0f };

public:
    explicit Camera(float focal, vec3f origin, vec3f look)
        : focal(focal)
        , origin(origin)
    {
        look_at(look);
    }

    void look_at(vec3f point);

    void rotate(affine3f const& rotate);
    void translate(affine3f const& translate);
};
