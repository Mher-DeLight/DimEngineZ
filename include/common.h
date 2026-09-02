#pragma once
#include <raylib.h>
#include <vector>

namespace DimEngineZ {
struct Transform {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 rotation{0.0f, 0.0f, 0.0f};
    Vector3 scale{1.0f, 1.0f, 1.0f};
};

struct DrawObject {
    Mesh mesh;
    Transform transform;
    void draw() const;

    DrawObject(const Mesh& mesh_, const Transform& transform_)
        : mesh(mesh_), transform(transform_) {}
};
} // namespace DimEngineZ