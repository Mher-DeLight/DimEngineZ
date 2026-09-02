#include "../include/common.h"
#include "raylib.h"
#include <iostream>

int main() {
    namespace dez = DimEngineZ;
    namespace dem = DimEngineZ::manager;

    dem::initWindow(800, 600, "3D Cube Renderer");

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};         // Camera up vector (rotation axis)
    camera.fovy = 45.0f;                             // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;          // Camera projection type

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto draw = dez::DrawObject(
        mesh, dez::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}));
    auto cube = dez::MoveObject(draw);

    cube.applyImpulse(Vector3({1.0f, 1.0f, 0.0f}));

    return dem::loop(60, [&]() {
        float delta = GetFrameTime();

        bool code = dem::render(RAYWHITE, true, [&]() {
            BeginMode3D(camera);
            cube.shape.draw();
            EndMode3D();

            return true;
        });

        cube.applyVelocity(Vector3{0.0f, -0.3f, 0.0});
        cube.tick(delta);

        return code;
    });
}
