#pragma once

#include <vector>
#include <array>

#include "../linear/vec.h"
#include "../linear/transform.h"
#include "../utility/objimporter.h"

struct TriangleDescriptor {
    size_t                normal;
    std::array<size_t, 3> vertices;
    std::array<size_t, 3> axes;
};

struct MeshInstance {
    std::vector<vec3f> vertices;
    std::vector<vec3f> normals;
    std::vector<vec3f> axes;

    std::vector<TriangleDescriptor> triangles;

public:
    explicit MeshInstance(OBJImporter::raw_obj_package<float> package);

    void transform(affine3f const& adapter);
};
