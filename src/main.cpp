#include "../include/common.h"
#include "raylib.h"

int main() {
    namespace dez = DimEngineZ;
    namespace dem = DimEngineZ::manager;

    dem::initWindow(800, 600, "3D Cube Renderer");

    // 2. Setup a 3D Perspective Camera
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};         // Camera up vector (rotation axis)
    camera.fovy = 45.0f;                             // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;          // Camera projection type

    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto cube = dez::DrawObject(
        mesh, dez::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}));

    // 3. Main Application Loop
    return dem::loop(60, [&]() {
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
