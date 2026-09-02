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
void loop(std::function<std::string()> func) {
    while (!WindowShouldClose()) {
        std::string err = func();
        if (!err.empty()) {
            TraceLog(LOG_FATAL, err.c_str());
            CloseWindow();
        }
    }
}
} // namespace DimEngineZ::manager