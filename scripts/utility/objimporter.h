#pragma once

#include <string_view>
#include <string>
#include <array>
#include <vector>

#include "../linear/vec.h"

struct OBJImporter {
    template <typename T>
    struct raw_obj_package {
        struct triangle_descriptor {
            size_t                normal;
            std::array<size_t, 3> vertices;
        };

        std::vector<vec<T, 3>>           vertices;
        std::vector<vec<T, 3>>           normals;
        std::vector<triangle_descriptor> triangles;
    };

    raw_obj_package<float> package;

    bool        exception;
    std::string file_tag;

public:
    OBJImporter() = default;

    void load(std::string_view filename);
    void load(std::istream& stream);
};
