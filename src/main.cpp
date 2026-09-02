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
    auto draw = dez::DrawObject(
        mesh, dez::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}), GREEN);
    auto cube1 = dez::PhysicsObject(std::move(draw));

    mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    draw = dez::DrawObject(
        mesh, dez::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}));
    auto cube2 = dez::PhysicsObject(std::move(draw));

    cube1.core.transform() = Vector3{-10.0f, 0.0f, 0.0f};
    cube2.core.transform() = Vector3{10.0f, 0.0f, 0.0f};

    cube1.core.applyImpulse(Vector3{0.0f, 10.0f, 0.0f});
    cube2.core.applyImpulse(Vector3{0.0f, 10.0f, 0.0f});

    cube1.core.velocity.x = 5.0f;
    cube2.core.velocity.x = -5.0f;

    return dem::fixedloop(
        120, 120,

        // Physics
        [&](float delta) {
            if (IsKeyDown(KEY_RIGHT)) {
                camera.target.x += 30.0f * delta;
            } else if (IsKeyDown(KEY_LEFT)) {
                camera.target.x -= 30.0f * delta;
            } else if (IsKeyDown(KEY_DOWN)) {
                camera.target.y -= 30.0f * delta;
            } else if (IsKeyDown(KEY_UP)) {
                camera.target.y += 30.0f * delta;
            }

            cube1.core.applyAcceleration(Vector3{0.0f, -9.81f, 0.0f}, delta);
            cube2.core.applyAcceleration(Vector3{0.0f, -9.81f, 0.0f}, delta);

            cube1.core.tick(delta);
            cube2.core.tick(delta);
            cube1.resolveCollision(cube2);

            std::cout << cube1.core.shape.transform.position.x << std::endl;

            return true;
        },

        // Rendering
        [&]() {
            return dem::render(RAYWHITE, true, [&]() {
                BeginMode3D(camera);
                cube1.core.shape.draw();
                cube2.core.shape.draw();
                EndMode3D();

                return true;
            });
        });
}
