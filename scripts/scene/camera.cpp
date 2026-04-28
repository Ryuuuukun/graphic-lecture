#include "camera.h"

#include "../linear/box.h"

void Camera::look_at(vec3f point) {
    auto direct = normalized(point - origin);

    float angle = SDL_acos(dot(direct, axis_z));
    if (std::abs(angle) > 10e-5) {
        vec3f axis  = normalized(cross(direct, axis_z));

        auto transform = fin::rotate(angle, axis);

        axis_x = axis_x | transform;
        axis_y = axis_y | transform;
        axis_z = axis_z | transform;
    }
}


void Camera::rotate(affine3f const& rotate) {
    axis_x = rotate(axis_x);
    axis_y = rotate(axis_y);
    axis_z = rotate(axis_z);
}

void Camera::translate(affine3f const& translate) {
    origin = translate(origin);
}