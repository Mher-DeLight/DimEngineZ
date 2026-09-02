#include "../include/common.h"
#include <stdexcept>

namespace DimEngineZ {

// == TRANSFORM ==
float Transform::magnitude() const {
    return Vector3Length(position);
}
float Transform::rotationMagnitude() const {
    return Vector3Length(rotation);
}
void Transform::rotateX(float amount) {
    rotation.x += amount;
}
void Transform::rotateY(float amount) {
    rotation.y += amount;
}
void Transform::rotateZ(float amount) {
    rotation.z += amount;
}

void Transform::move(const Vector3& displacement) {
    position += displacement;
}
void Transform::moveX(float amount) {
    position.x += amount;
}
void Transform::moveY(float amount) {
    position.y += amount;
}
void Transform::moveZ(float amount) {
    position.z += amount;
}

void Transform::goTo(const Vector3& newposition) {
    position = newposition;
}
void Transform::goToX(float amount) {
    position.x = amount;
}
void Transform::goToY(float amount) {
    position.y = amount;
}
void Transform::goToZ(float amount) {
    position.z = amount;
}

// == DRAW OBJECT ==
void DrawObject::draw() const {
    float angle = transform.rotationMagnitude();

    Vector3 axis = {0.0f, 1.0f, 0.0f};

    DrawModelEx(model, transform.position, axis, angle, transform.scale, color);
}

// == MOVE OBJECT ==
void MovementObject::tick(float delta_) {
    transform().position += velocity * delta_;

    auto applyDrag = [this, delta_](float& v) {
        if (v > 0.0f)
            v = std::max(v - drag * delta_, 0.0f);
        else if (v < 0.0f)
            v = std::min(v + drag * delta_, 0.0f);
    };

    applyDrag(velocity.x);
    applyDrag(velocity.y);
    applyDrag(velocity.z);
}
void MovementObject::applyVelocity(const Vector3& applied) {
    velocity += applied;
}
void MovementObject::setVelocity(const Vector3& newvelocity) {
    velocity = newvelocity;
}

void MovementObject::applyForce(const Vector3& force, float delta) {
    velocity += (force / mass) * delta;
}
void MovementObject::applyAcceleration(const Vector3& acceleration, float delta) {
    velocity += acceleration * delta;
}
void MovementObject::applyImpulse(const Vector3& impulse) {
    velocity += impulse / mass;
}

// == COLLISION BOX ==
bool CollisionBox::colliding_with(const CollisionBox& other) const {
    return CheckCollisionBoxes(box, other.box);
}

} // namespace DimEngineZ

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

bool render(Color background, bool clear, FuncitonCallback func) {
    BeginDrawing();
    if (clear)
        ClearBackground(background);

    bool code = func();
    EndDrawing();
    return code;
}
} // namespace DimEngineZ::manager