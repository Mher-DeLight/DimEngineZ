#include "../include/common.h"
#include "raylib.h"
#include <iostream>

int main() {
    namespace dez = DimEngineZ;
    namespace dem = DimEngineZ::manager;

    dem::initWindow(1000, 800, "3D Cube Renderer");

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 5.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation axis)
    camera.fovy = 45.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto draw = dez::DrawObject(
        mesh, dez::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}));
    auto cube = dez::MovementObject(draw);

    return dem::fixedloop(
        120, 120,

        // Physics
        [&](float delta) {
            if (IsKeyPressed(KEY_SPACE)) {
                cube.setVelocity(Vector3({0.0f, 7.5f, 0.0f}));
            }
            cube.applyAcceleration(Vector3{0.0f, -9.81f, 0.0f}, delta);
            cube.tick(delta);

            return true;
        },

        // Rendering
        [&]() {
            return dem::render(RAYWHITE, true, [&]() {
                BeginMode3D(camera);
                cube.shape.draw();
                EndMode3D();

                return true;
            });
        });
}
