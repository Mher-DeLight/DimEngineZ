#include "../include/common.h"
#include "raylib.h"

int main() {
    // 1. Initialize Window & Context
    InitWindow(800, 600, "Raylib 3D Example");

    // 2. Setup a 3D Perspective Camera
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};         // Camera up vector (rotation axis)
    camera.fovy = 45.0f;                             // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;          // Camera projection type

    SetTargetFPS(60);

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto cube = DimEngineZ::DrawObject(
        mesh, DimEngineZ::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}));

    // 3. Main Application Loop
    return DimEngineZ::manager::loop([&]() {
        float delta = GetFrameTime();

        // 4. Render Cycle
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        cube.draw();
        EndMode3D();

        cube.transform.rotateX(10.0 * delta);

        DrawText("Simple 3D Cube with Raylib", 10, 10, 20, DARKGRAY);
        EndDrawing();

        return true;
    });
}
