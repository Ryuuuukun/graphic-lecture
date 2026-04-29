#pragma once

#include <memory>

#include "bvh.h"
#include "../linear/ray.h"
#include "../linear/sphere.h"
#include "../render/raycasthit.h"

struct Model {
    static constexpr size_t DEFAULT_CUTS_BVH = 1;
    static constexpr float EPS = 10e-5;

    using ShellVolume = sphere3f;

    std::shared_ptr<MeshInstance> instance;
    std::shared_ptr<Material>     material;

    ShellVolume              shell;
    std::vector<ShellVolume> shells;

public:
    Model(std::shared_ptr<MeshInstance> instance);

    Model(Model const& other);
    Model& operator=(Model const& other);

    Model(Model&& other) = default;
    Model& operator=(Model&& other) = default;

    void trace(ray3f const& ray, RaycastHit& raycast) const;

    void transform(affine3f const& affine);

private:
    void trace_impl(TriangleDescriptor const& triangle, ray3f const& ray, RaycastHit& raycast) const;

    void build_shells();
};