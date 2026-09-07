#include "../include/manager.h"
#include <stdexcept>

namespace DimEngineZ::manager {
using FuncitonCallback = std::function<bool()>;
using FixedCallback = std::function<bool(float)>;

void initWindow(int width, int height, const std::string& title) {
    InitWindow(width, height, title.c_str());
}
int loop(int targetFPS, FuncitonCallback func) {
    SetTargetFPS(targetFPS);
    bool status = true;
    while (!WindowShouldClose() && status) {
        status = func();
    }
    CloseWindow();
    return status ? 0 : -1;
}
int fixedloop(int targetFPS, float fixed_delta_inverse, std::function<bool(float)> physics,
              FuncitonCallback render) {
    SetTargetFPS(targetFPS);

    const float fixed_delta = 1.0f / fixed_delta_inverse;
    float accumulator = 0.0f;

    bool status = true;

    while (!WindowShouldClose() && status) {
        accumulator += GetFrameTime();

        while (accumulator >= fixed_delta) {
            status = physics(fixed_delta);
            accumulator -= fixed_delta;
        }
        if (status)
            status = render();
    }

    CloseWindow();
    return status ? 0 : -1;
}
int fixedloop(int targetFPS, std::function<bool(float)> physics, FuncitonCallback render) {
    SetTargetFPS(targetFPS);

    const float fixed_delta = 1.0f / targetFPS;
    float accumulator = 0.0f;

    bool status = true;

    while (!WindowShouldClose() && status) {
        accumulator += GetFrameTime();

        while (accumulator >= fixed_delta) {
            status = physics(fixed_delta);
            accumulator -= fixed_delta;
        }
        if (status)
            status = render();
    }

    CloseWindow();
    return status ? 0 : -1;
}

bool render(Color background, bool clear, FuncitonCallback func) {
    BeginDrawing();
    if (clear)
        ClearBackground(background);

    bool code = func();
    EndDrawing();
    return code;
}

void registerObject(managedObject obj) {
    handledObjects.push_back(obj);
    if (std::holds_alternative<PhysicsObject*>(obj)) {
        // register at the physics registry too
        physics::registerObject(std::get<PhysicsObject*>(obj));
    }
}
void tick(float delta) {
    for (auto& obj : handledObjects) {
        tickObject(obj, delta);
    }
}
void tickObject(managedObject obj, float delta) {
    if (std::holds_alternative<PhysicsObject*>(obj)) {
        auto cst = std::get<PhysicsObject*>(obj);
        cst->tick(delta);
        physics::resolveCollisionForObject(cst);
    } else if (std::holds_alternative<DrawObject*>(obj)) {
        auto cst = std::get<DrawObject*>(obj);
        cst->draw();
    } else if (std::holds_alternative<MovementObject*>(obj)) {
        auto cst = std::get<MovementObject*>(obj);
        cst->shape.draw();
    } else {
        throw std::runtime_error("DimEngineZ: invalid type in handledObjects for object handler");
    }
}

} // namespace DimEngineZ::manager
