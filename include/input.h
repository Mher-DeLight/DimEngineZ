#pragma once
#include "common.h"

namespace DimEngineZ::input {

int getAxis(int keyNeg = KEY_LEFT, int keyPos = KEY_RIGHT);
Vec2 getVector(int keyNegX = KEY_LEFT, int keyPosX = KEY_RIGHT, int keyNegY = KEY_DOWN,
               int keyPosY = KEY_UP);
Vec3 getVector3(int keyNegX, int keyPosX, int keyNegY, int keyPosY, int keyNegZ, int keyPosZ);
}; // namespace DimEngineZ::input