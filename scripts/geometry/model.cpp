#include "model.h"

#include <cmath>

#include "SDL_log.h"

Model::Model(std::shared_ptr<MeshInstance> instance)
    : instance(std::move(instance))
{
    shell = ShellVolume::from_bound(this->instance->vertices);
    build_shells();
}

Model::Model(Model const& other)
    : instance(std::make_shared<MeshInstance>(*other.instance))
    , shell(other.shell)
{
    build_shells();
}

Model& Model::operator=(Model const& other) {
    if (this != &other) {
        instance = std::make_shared<MeshInstance>(*other.instance);
        shell    = other.shell;

        build_shells();
    }

    return *this;
}

void Model::trace(ray3f const& ray, RaycastHit& raycast) const {
    if (shell.intersect(ray)) {
        // auto node = bvh.find_volume(ray);
        // if (node != nullptr) {
        //     for (auto const& index : node->triangles) {
        //         if (shells[index].intersect(ray)) {
        //             trace_impl(instance->triangles[index], ray, raycast);
        //         }
        //     }
        // }

        for (size_t index = 0; index < instance->triangles.size(); ++index) {
            if (shells[index].intersect(ray)) {
                trace_impl(instance->triangles[index], ray, raycast);
            }
        }
    }
}

void Model::trace_impl(TriangleDescriptor const& triangle, ray3f const& ray, RaycastHit& raycast) const {
    vec3f const& vert_a = instance->vertices[triangle.vertices[0]];
    vec3f const& vert_b = instance->vertices[triangle.vertices[1]];
    vec3f const& vert_c = instance->vertices[triangle.vertices[2]];

    vec3f const& axes_a = instance->axes[triangle.axes[0]];
    vec3f const& axes_b = instance->axes[triangle.axes[1]];
    vec3f const& axes_c = instance->axes[triangle.axes[2]];

    vec3f const& normal = instance->normals[triangle.normal];

    if (dot(normal, ray.delta) < 0) {
        float param = (dot(vert_a, normal) - dot(ray.origin, normal)) / dot(ray.delta, normal);
        vec3f point = ray.origin + param * ray.delta;

        bool sign_a = std::signbit(dot(axes_a, point - vert_a) - EPS);
        bool sign_b = std::signbit(dot(axes_b, point - vert_b) - EPS);
        bool sign_c = std::signbit(dot(axes_c, point - vert_c) - EPS);

        if (sign_a == sign_b && sign_b == sign_c) {
            raycast.send(param, point, [&normal, material = material](auto& storage) {
                storage.normal   = &normal;
                storage.material = material.get();
            });
        }
    }
}

void Model::transform(affine3f const& adapter) {
    instance->transform(adapter);

    shell = ShellVolume::from_bound(this->instance->vertices);
    build_shells();
}

void Model::build_shells() {
    shells.clear();
    for (auto const& triangle : instance->triangles) {
        shells.push_back(ShellVolume::from_bound({
            instance->vertices[triangle.vertices[0]],
            instance->vertices[triangle.vertices[1]],
            instance->vertices[triangle.vertices[2]]
        }));
    }
}





