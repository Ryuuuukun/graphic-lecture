#include "mesh.h"

#include <algorithm>

MeshInstance::MeshInstance(OBJImporter::raw_obj_package<float> package)
    : vertices(std::move(package.vertices))
    , normals(std::move(package.normals))
{
    for (auto const& triangle : package.triangles) {
        vec3f const& vert_a = this->vertices[triangle.vertices[0]];
        vec3f const& vert_b = this->vertices[triangle.vertices[1]];
        vec3f const& vert_c = this->vertices[triangle.vertices[2]];

        vec3f const& normal = this->normals[triangle.normal];

        axes.push_back(normalized(cross(vert_b - vert_a, normal)));
        axes.push_back(normalized(cross(vert_c - vert_b, normal)));
        axes.push_back(normalized(cross(vert_a - vert_c, normal)));

        triangles.push_back({
            .normal   = triangle.normal,
            .vertices = triangle.vertices,
            .axes     = {
                axes.size() - 3,
                axes.size() - 2,
                axes.size() - 1
            }
        });
    }
}

void MeshInstance::transform(affine3f const& adapter) {
    std::ranges::transform(vertices, vertices.begin(), adapter);
    for (auto triangle : triangles) {
        auto const& vert_a = vertices[triangle.vertices[0]];
        auto const& vert_b = vertices[triangle.vertices[1]];
        auto const& vert_c = vertices[triangle.vertices[2]];

        normals[triangle.normal] = normalized(cross(vert_b - vert_a, vert_c - vert_a));
        auto const& normal = normals[triangle.normal];

        axes[triangle.axes[0]] = normalized(cross(vert_b - vert_a, normal));
        axes[triangle.axes[1]] = normalized(cross(vert_c - vert_b, normal));
        axes[triangle.axes[2]] = normalized(cross(vert_a - vert_c, normal));
    }
}

