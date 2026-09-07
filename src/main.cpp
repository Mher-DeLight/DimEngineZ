#include "../include/common.h"
#include "raylib.h"
#include <iostream>

int main() {
    namespace dez = DimEngineZ;
    namespace dem = DimEngineZ::manager;

    dem::initWindow(1000, 800, "3D Cube Renderer");

    dez::Camera camera({0.0f, 7.5f, 10.0f}, 90.0f, {0.0f, -1.0f, -1.0f});

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

    return dem::fixedloop(
        120, 120,

        // Physics
        [&](float delta) {
            constexpr float SPEED = 12.0f;
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
                camera.moveX(CAM_SPEED * delta);
            } else if (IsKeyDown(KEY_A)) {
                camera.moveX(-CAM_SPEED * delta);
            }
            if (IsKeyDown(KEY_S)) {
                camera.moveZ(CAM_SPEED * delta);
            } else if (IsKeyDown(KEY_W)) {
                camera.moveZ(-CAM_SPEED * delta);
            }
            if (IsKeyDown(KEY_Q)) {
                camera.moveY(CAM_SPEED * delta);
            } else if (IsKeyDown(KEY_E)) {
                camera.moveY(-CAM_SPEED * delta);
            }

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
