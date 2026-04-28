#include "scene.h"

void Scene::load_models(std::vector<std::string_view> files) {
    OBJImporter importer;
    for (auto const& filename : files) {
        importer.load(filename);
        if (!importer.exception) {
            models.emplace_back(std::make_shared<MeshInstance>(importer.package));
        }
    }
}

void Scene::load_models(std::vector<model_descriptor> descriptors) {
    OBJImporter importer;
    for (auto const& [filename, transform, material] : descriptors) {
        importer.load(filename);
        if (!importer.exception) {
            models.emplace_back(std::make_shared<MeshInstance>(importer.package));
            models.back().transform(transform);
            models.back().material = material;
        }
    }
}


