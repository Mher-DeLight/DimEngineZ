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
    camera.fovy = 90.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto draw = dez::DrawObject(mesh, dez::Transform(), GREEN);
    auto cube = dez::PhysicsObject(std::move(draw), 0.5f);
    cube.core.transform().position = Vector3{0.0f, 5.0f, 0.0f};
    DimEngineZ::physics::registerObject(&cube);

    mesh = GenMeshCube(20.0f, 1.0f, 20.0f);
    draw = dez::DrawObject(mesh, dez::Transform({0.0f, -0.5f, 0.0f}), BROWN);
    auto cube2 = dez::PhysicsObject(std::move(draw));
    DimEngineZ::physics::registerObject(&cube2);

    return dem::fixedloop(
        120, 120,

        // Physics
        [&](float delta) {
            constexpr int SPEED = 10.0f;
            constexpr float JUMP_FORCE = 10.0f;
            constexpr float GRAVITY = -20.0f;

            if (IsKeyDown(KEY_RIGHT)) {
                cube.core.transform().position.x += SPEED * delta;
            } else if (IsKeyDown(KEY_LEFT)) {
                cube.core.transform().position.x -= SPEED * delta;
            } else if (IsKeyDown(KEY_DOWN)) {
                cube.core.transform().position.z += SPEED * delta;
            } else if (IsKeyDown(KEY_UP)) {
                cube.core.transform().position.z -= SPEED * delta;
            }
            if (IsKeyPressed(KEY_SPACE) && cube.core.transform().position.y <= 0.0f) {
                cube.core.applyImpulse(Vector3{0.0f, JUMP_FORCE, 0.0f});
            }
            cube.core.applyAcceleration(Vector3{0.0f, GRAVITY, 0.0f}, delta);

            std::cout << "x: " << cube.core.transform().position.x
                      << " y: " << cube.core.transform().position.y
                      << " z: " << cube.core.transform().position.z << std::endl;

            DimEngineZ::physics::tick(delta);
            return true;
        },

        // Rendering
        [&]() {
            return dem::render(RAYWHITE, true, [&]() {
                BeginMode3D(camera);
                cube.core.shape.draw();
                cube2.core.shape.draw();
                EndMode3D();

                return true;
            });
        });
}
