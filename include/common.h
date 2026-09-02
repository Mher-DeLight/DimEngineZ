#pragma once
#include <functional>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

namespace DimEngineZ {

struct Transform {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 rotation{0.0f, 0.0f, 0.0f};
    Vector3 scale{1.0f, 1.0f, 1.0f};

    float magnitude() const;
    float rotationMagnitude() const;

    void rotateX(float amount);
    void rotateY(float amount);
    void rotateZ(float amount);

    Transform(Vector3 position_ = {0.0f, 0.0f, 0.0f}, Vector3 rotation_ = {0.0f, 0.0f, 0.0f},
              Vector3 scale_ = {1.0f, 1.0f, 1.0f})
        : position(position_), rotation(rotation_), scale(scale_) {}
};
struct DrawObject {
    Model model;
    Transform transform;
    Color color;

    DrawObject(const Mesh& mesh_, const Transform& transform_, const Color& color_ = RED)
        : model(LoadModelFromMesh(mesh_)), transform(transform_), color(color_) {}

    ~DrawObject() {
        UnloadModel(model);
    }

    void draw() const;
};
} // namespace DimEngineZ

namespace DimEngineZ::manager {

void initWindow(int width, int height, const std::string& title);

using FuncitonCallack = std::function<bool()>;
int loop(int targetFPS, FuncitonCallack func);
} // namespace DimEngineZ::manager