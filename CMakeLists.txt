cmake_minimum_required(VERSION 3.22)
project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} scripts/main.cpp

        scripts/utility/context.cpp
        scripts/utility/context.h

        scripts/linear/vec.h
        scripts/linear/mat.h
        scripts/linear/ray.h

        scripts/component/color.h
        scripts/linear/qut.h
        scripts/linear/sphere.h
        scripts/linear/box.h
        scripts/geometry/mesh.h
        scripts/geometry/bvh.h
        scripts/geometry/bvh.cpp
        scripts/geometry/model.h
        scripts/geometry/model.cpp
        scripts/utility/buffer.h
        scripts/utility/buffer.cpp
        scripts/render/tracing.h
        scripts/utility/benchmark.h
        scripts/scene/camera.h
        scripts/render/algorithm.h
        scripts/linear/transform.h
        scripts/render/raycasthit.h
        scripts/utility/objimporter.h
        scripts/utility/objimporter.cpp
        scripts/geometry/mesh.cpp
        scripts/scene/scene.h
        scripts/scene/scene.cpp
        scripts/scene/camera.cpp
        scripts/postprocess/fxaa.h
        scripts/postprocess/vignette.h
        scripts/component/light.h
        scripts/component/material.h
)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})