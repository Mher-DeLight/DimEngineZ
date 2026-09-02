#include "../include/common.h"
#include <iostream>
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
void CollisionBox::update(const Transform& transform) {
    Vector3 center = {(box.min.x + box.max.x) * 0.5f, (box.min.y + box.max.y) * 0.5f,
                      (box.min.z + box.max.z) * 0.5f};

    Vector3 offset = Vector3Subtract(transform.position, center);

    box.min = Vector3Add(box.min, offset);
    box.max = Vector3Add(box.max, offset);
}

// == PHYSICS OBJECT ==
void PhysicsObject::tick(float delta) {
    core.tick(delta);

    Vector3 position = core.transform().position;

    Vector3 center = {(collision.box.min.x + collision.box.max.x) * 0.5f,
                      (collision.box.min.y + collision.box.max.y) * 0.5f,
                      (collision.box.min.z + collision.box.max.z) * 0.5f};

    Vector3 offset = Vector3Subtract(position, center);

    collision.box.min = Vector3Add(collision.box.min, offset);
    collision.box.max = Vector3Add(collision.box.max, offset);
}
void PhysicsObject::resolveCollision(PhysicsObject& other) {
    collision.update(core.transform());
    other.collision.update(other.core.transform());

    if (!collision.colliding_with(other.collision))
        return;

    BoundingBox& a = collision.box;
    BoundingBox& b = other.collision.box;

    float overlapX = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
    float overlapY = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
    float overlapZ = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

    // find the axis with the smallest penetration, that's the axis we'll resolve the collision on.
    // (aabb)
    if (overlapX <= overlapY && overlapX <= overlapZ) { // x axis
        float direction =
            core.transform().position.x < other.core.transform().position.x ? -1.0f : 1.0f;

        core.transform().moveX(direction * overlapX * 0.5f);
        other.core.transform().moveX(-direction * overlapX * 0.5f);

        core.velocity.x *= -1.0f * bounce;
        other.core.velocity.x *= -1.0f * bounce;
    } else if (overlapY <= overlapZ) { // y axis
        float direction =
            core.transform().position.y < other.core.transform().position.y ? -1.0f : 1.0f;

        core.transform().moveY(direction * overlapY * 0.5f);
        other.core.transform().moveY(-direction * overlapY * 0.5f);

        core.velocity.y *= -1.0f * bounce;
        other.core.velocity.y *= -1.0f * bounce;
    } else { // z axis
        float direction =
            core.transform().position.z < other.core.transform().position.z ? -1.0f : 1.0f;

        core.transform().moveZ(direction * overlapZ * 0.5f);
        other.core.transform().moveZ(-direction * overlapZ * 0.5f);

        core.velocity.z *= -1.0f * bounce;
        other.core.velocity.z *= -1.0f * bounce;
    }
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
namespace DimEngineZ::physics {
void registerObject(PhysicsObject* object) {
    objects.push_back(object);
}
void tick(float delta) {
    for (int i = 0; i < objects.size(); i++) {
        for (int j = 0; j < objects.size() || j == i; j++) {
            objects[i]->resolveCollision(*objects[j]);
            objects[i]->tick(delta);
        }
    }
}

} // namespace DimEngineZ::physics