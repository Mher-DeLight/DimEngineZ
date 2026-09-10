#include <DimEngineZ.h>
#include <iostream>

int main() {
    namespace dez = DimEngineZ;

    dez::manager::initWindow(1000, 800, "3D Cube Renderer");

    dez::Camera camera(
        {0.0f, 5.0f, 10.0f},
        dez::CameraOptions{.fovy = 90.0f, .direction = {0.0f, -1.0f, -1.0f}, .up = dez::Vec3::UP},
        MAIN_CAMERA);
    camera.setTarget(Vector3{0.0f, 0.0f, 0.0f});

    Mesh mesh = GenMeshCube(2.0f, 4.0f, 2.0f);
    auto draw = dez::DrawObject(mesh, dez::Transform(), GREEN);
    auto player = dez::PhysicsObject(std::move(draw), 0.0f);
    player.transform.position = Vector3{0.0f, 5.0f, 0.0f};
    player.core.mass = 50.0f;

    mesh = GenMeshCube(20.0f, 1.0f, 20.0f);
    draw = dez::DrawObject(mesh, dez::Transform({0.0f, -0.5f, 0.0f}), BROWN);
    auto ground = dez::PhysicsObject(std::move(draw));
    ground.setStatic(true);

    int exitcode = mainloop(60, {
        constexpr float JUMP_FORCE = 242.0f;
        constexpr float GRAVITY = 9.81f;
        constexpr float SPEED = 10.0f;
        dez::Vec2 input = dez::input::getVector2() * SPEED;

        if (dez::input::isKeyPressed(KEY_SPACE) && player.isOnGround) {
            player.core.applyImpulse(Vector3{0.0f, JUMP_FORCE, 0.0f});
        }
        player.core.setHorizontalVelocity(input);
        player.core.applyAcceleration(Vector3{0.0f, -GRAVITY, 0.0f}, delta);

        dez::manager::tick(delta);
        if (dez::input::isKeyPressed(KEY_Q)) {
            exitwcode(0);
        }
    });

    dez::logger::flushLog("==== EXIT WITH CODE: " + std::to_string(dez::manager::exitCode) +
                          " =====");
    return exitcode;
}
