#include "bvh.h"

#include <algorithm>
#include <ranges>
#include <stack>

#include "SDL_log.h"

bool BVH::any_hits(bbox3f const& box, MeshInstance const& instance) {
    return std::ranges::any_of(instance.triangles,
                [&box, &instance](auto& triangle) {
                    return box.hits(
                            instance.vertices[triangle.vertices[0]],
                            instance.vertices[triangle.vertices[1]],
                            instance.vertices[triangle.vertices[2]]);
                });
}

std::vector<size_t> BVH::find_hits(bbox3f const& box, MeshInstance const& instance) {
    std::vector<size_t> triangles;
    for (auto const& [index, triangle] : instance.triangles | std::views::enumerate) {
        if (box.hits(
            instance.vertices[triangle.vertices[0]],
            instance.vertices[triangle.vertices[1]],
            instance.vertices[triangle.vertices[2]]))
        {
            triangles.push_back(index);
        }
    }

    return triangles;
}

void BVH::build(size_t cuts, MeshInstance const& instance) {
    root = std::make_shared<Node>(bbox3f::from_bound(instance.vertices));
    build_impl(root, cuts, instance);
}

void BVH::build_impl(std::shared_ptr<Node> node, size_t cuts, MeshInstance const& instance) {
    vec3f axis; axis[cuts % 3 - 1] = size(node->box)[cuts % 3 - 1];

    std::array boxes = {
        bbox3f{ node->box.min, node->box.max - axis }, bbox3f{ node->box.min + axis, node->box.max }
    };
    for (auto [i, box] : boxes | std::views::enumerate) {
        auto triangles = find_hits(box, instance);
        if (!triangles.empty()) {
            auto child = std::make_shared<Node>(box);
            if (cuts - 1 == 0) {
                child->triangles = std::move(triangles);
            } else {
                build_impl(child, cuts - 1, instance);
            }

            if (i == 0) {
                node->left = child;
            } else {
                node->right = child;
            }
        }
    }
}

std::shared_ptr<BVH::Node> BVH::find_volume(ray3f const& ray) const {
    std::stack<std::shared_ptr<Node>> cache({ root });
    while (!cache.empty()) {
        auto node = cache.top(); cache.pop();

        if (!node->triangles.empty()) {
            return node;
        }

        auto min_param = std::numeric_limits<float>::infinity();
        if (false && node->left != nullptr) {
            auto params = node->left->box.get_params(ray);
            if (params.y > std::max(params.x, 0.0f)) {
                min_param = std::max(params.x, 0.0f);
                cache.push(node->left);
            }
        }

        if (node->right != nullptr) {
            auto params = node->right->box.get_params(ray);
            if (params.y > std::max(params.x, 0.0f)) {
                if (min_param == std::numeric_limits<float>::infinity()) {
                    cache.push(node->right);
                } else {
                    if (std::max(params.x, 0.0f) < min_param) {
                        cache.push(node->right);
                    } else {
                        cache.pop();
                        cache.push(node->right);
                        cache.push(node->left);
                    }
                }
            }
        }
    }

    return nullptr;
}

void print_all_impl(std::shared_ptr<BVH::Node> node, size_t cut = 0) {
    SDL_Log("[cut %llu]  box: min(%f %f %f) max(%f %f %f)", cut, node->box.min.x, node->box.min.y, node->box.min.z, node->box.max.x, node->box.max.y, node->box.max.z);
    SDL_Log("--------------------");
    if (node->left != nullptr) {
        print_all_impl(node->left, cut + 1);
    }
    if (node->right != nullptr) {
        print_all_impl(node->right, cut + 1);
    }
}

void BVH::print_all() const {
    print_all_impl(root);
}






