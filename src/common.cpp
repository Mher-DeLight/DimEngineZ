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

// == DRAW OBJECT ==
void DrawObject::draw() const {
    float angle = transform.rotationMagnitude();

    Vector3 axis = {0.0f, 1.0f, 0.0f};

    DrawModelEx(model, transform.position, axis, angle, transform.scale, color);
}

} // namespace DimEngineZ

namespace DimEngineZ::manager {
int loop(std::function<bool()> func) {
    bool status = true;
    while (!WindowShouldClose() && status) {
        status = func();
    }
    CloseWindow();
    return status ? 0 : -1;
}
} // namespace DimEngineZ::manager