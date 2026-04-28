#include "objimporter.h"

#include <SDL_log.h>
#include <fstream>
#include <sstream>

void OBJImporter::load(std::string_view path) {
    std::ifstream file(path.data());
    if (file.is_open()) {
        this->file_tag = path;
        load(file);
    } else {
        exception = true;
        SDL_Log("OBJLoader: no file named \"%s\" was found.", path.data());
    }
}

void OBJImporter::load(std::istream& stream) {
    exception = false;
    package = { };

    std::string line;
    while (std::getline(stream, line)) {
        std::stringstream ss(line);

        std::string op; ss >> op;
        if (op == "v") {
            vec3f vec;
            ss >> vec.x >> vec.y >> vec.z;
            package.vertices.push_back(vec);
        } else if (op == "vn") {
            vec3f vec;
            ss >> vec.x >> vec.y >> vec.z;
            package.normals.push_back(vec);
        } else if (op == "f") {
            raw_obj_package<float>::triangle_descriptor triangle;

            char slash;
            size_t vertex;
            size_t normal;
            size_t index = 0;
            while (!ss.eof()) {
                ss >> vertex >> slash >> slash >> normal;
                if (index < 3) {
                    triangle.vertices[index++] = vertex - 1;
                    triangle.normal = normal - 1;
                } else {
                    exception = true;
                    SDL_Log("OBJLoader [%s]: the mesh is not triangulated", file_tag.data());
                }
            }

            package.triangles.push_back(triangle);
        }
    }
}

