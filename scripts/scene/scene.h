#pragma once

#include <vector>

#include "../geometry/model.h"

struct Scene {
    struct model_descriptor {
        std::string               filename;
        affine3f                  transform;
        std::shared_ptr<Material> material;
    };

    std::vector<Model> models;

public:
    explicit Scene() = default;

    void load_models(std::vector<std::string_view> files);
    void load_models(std::vector<model_descriptor> descriptors);
};
