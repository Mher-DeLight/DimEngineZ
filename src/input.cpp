#include "../include/input.h"

namespace DimEngineZ::input {

int getAxis(int keyNeg, int keyPos) {
    return IsKeyDown(keyPos) - IsKeyDown(keyNeg);
}
Vec2 getVector(int keyNegX, int keyPosX, int keyNegY, int keyPosY) {
    return Vec2{static_cast<float>(getAxis(keyNegX, keyPosX)),
                static_cast<float>(getAxis(keyPosY, keyNegY))};
}
Vec3 getVector3(int keyNegX, int keyPosX, int keyNegY, int keyPosY, int keyNegZ, int keyPosZ) {
    return Vec3{static_cast<float>(getAxis(keyNegX, keyPosX)),
                static_cast<float>(getAxis(keyPosY, keyNegY)),
                static_cast<float>(getAxis(keyPosZ, keyNegZ))};
}

} // namespace DimEngineZ::input