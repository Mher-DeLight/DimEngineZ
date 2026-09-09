#include "../include/DimEngineZ.h"
#include "raylib.h"
#include <iostream>

int main() {
    namespace dez = DimEngineZ;

    dez::manager::initWindow(1000, 800, "3D Cube Renderer");

    dez::Camera camera(
        {0.0f, 5.0f, 10.0f},
        dez::CameraOptions{.fovy = 90.0f, .direction = {0.0f, -1.0f, -1.0f}, .up = dez::Vec3::UP},
        MAIN_CAMERA);
    camera.setTarget(Vector3{0.0f, 0.0f, 0.0f});

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto draw = dez::DrawObject(mesh, dez::Transform(), GREEN);
    auto object = dez::PhysicsObject(std::move(draw), 0.0f);
    object.transform.position = Vector3{0.0f, 5.0f, 0.0f};

    mesh = GenMeshCube(20.0f, 1.0f, 20.0f);
    draw = dez::DrawObject(mesh, dez::Transform({0.0f, -0.5f, 0.0f}), BROWN);
    auto ground = dez::PhysicsObject(std::move(draw));
    ground.setStatic(true);

    return dez::manager::main(60, [&](float delta) {
        constexpr float LAUNCH_FORCE = 15.0f;
        constexpr float GRAVITY = 20.0f;
        constexpr float SPEED = 10.0f;
        dez::Vec2 input = dez::input::getVector() * SPEED;

        if (IsKeyPressed(KEY_SPACE) && object.is_on_ground) {
            object.core.applyImpulse(Vector3{0.0f, LAUNCH_FORCE, 0.0f});
        }
        object.core.setVelocity(dez::Vec3{input.x, object.core.velocity.y, input.y});
        object.core.applyAcceleration(Vector3{0.0f, -GRAVITY, 0.0f}, delta);

        if (IsKeyPressed(KEY_Q)) {
            return false; // exit
        }

        DimEngineZ::manager::tick(delta);
        return true;
    });
}
