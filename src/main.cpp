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
    auto player = dez::PhysicsObject(std::move(draw), 0.5f);
    player.core.transform().position = Vector3{0.0f, 5.0f, 0.0f};
    DimEngineZ::physics::registerObject(&player);

    mesh = GenMeshCube(20.0f, 1.0f, 20.0f);
    draw = dez::DrawObject(mesh, dez::Transform({0.0f, -0.5f, 0.0f}), BROWN);
    auto ground = dez::PhysicsObject(std::move(draw));
    ground.is_static = true;
    DimEngineZ::physics::registerObject(&ground);

    Vector3 camdir = Vector3Subtract(camera.target, camera.position);
    return dem::fixedloop(
        120, 120,

        // Physics
        [&](float delta) {
            constexpr float SPEED = 10.0f;
            constexpr float JUMP_FORCE = 10.0f;
            constexpr float GRAVITY = 20.0f;
            constexpr float CAM_SPEED = 10.0f;

            if (IsKeyDown(KEY_RIGHT)) {
                player.core.transform().position.x += SPEED * delta;
            } else if (IsKeyDown(KEY_LEFT)) {
                player.core.transform().position.x -= SPEED * delta;
            }
            if (IsKeyDown(KEY_DOWN)) {
                player.core.transform().position.z += SPEED * delta;
            } else if (IsKeyDown(KEY_UP)) {
                player.core.transform().position.z -= SPEED * delta;
            }

            if (IsKeyDown(KEY_D)) {
                camera.position.x += CAM_SPEED * delta;
            } else if (IsKeyDown(KEY_A)) {
                camera.position.x -= CAM_SPEED * delta;
            }
            if (IsKeyDown(KEY_S)) {
                camera.position.z += CAM_SPEED * delta;
            } else if (IsKeyDown(KEY_W)) {
                camera.position.z -= CAM_SPEED * delta;
            }
            if (IsKeyDown(KEY_Q)) {
                camera.position.y += CAM_SPEED * delta;
            } else if (IsKeyDown(KEY_E)) {
                camera.position.y -= CAM_SPEED * delta;
            }
            camera.target = Vector3Add(camera.position, camdir);

            if (IsKeyPressed(KEY_SPACE) && player.is_on_ground) {
                player.core.applyImpulse(Vector3{0.0f, JUMP_FORCE, 0.0f});
            }
            player.core.applyAcceleration(Vector3{0.0f, -GRAVITY, 0.0f}, delta);

            DimEngineZ::physics::tick(delta);
            return true;
        },

        // Rendering
        [&]() {
            return dem::render(RAYWHITE, true, [&]() {
                BeginMode3D(camera);
                player.core.shape.draw();
                ground.core.shape.draw();
                DrawCubeWires(player.core.transform().position, 2.0f, 2.0f, 2.0f, BLACK);
                DrawCubeWires(ground.core.transform().position, 20.0f, 1.0f, 20.0f, BLACK);
                EndMode3D();

                return true;
            });
        });
}
