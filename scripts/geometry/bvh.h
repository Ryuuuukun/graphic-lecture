#pragma once

#include <memory>
#include <vector>

#include "mesh.h"
#include "../linear/box.h"

struct BVH {
    struct Node {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        std::vector<size_t> triangles;

        bbox3f box;

    public:
        Node() = default;

        Node(bbox3f box)
            : box(box)
        {}
    };

    std::shared_ptr<Node> root;

public:
    BVH() = default;

    static bool any_hits(bbox3f const& box, MeshInstance const& instance);
    static std::vector<size_t> find_hits(bbox3f const& box, MeshInstance const& instance);

    void build(size_t depth, MeshInstance const& instance);

    std::shared_ptr<Node> find_volume(ray3f const& ray) const;

    void print_all() const;

private:
    void build_impl(std::shared_ptr<Node> node, size_t deepth, MeshInstance const& instance);
};



