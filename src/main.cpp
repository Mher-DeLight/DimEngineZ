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
    Model cube = LoadModelFromMesh(mesh);
    float rotation = 0.0f;

    // 3. Main Application Loop
    while (!WindowShouldClose()) {
        // Update variables or handle inputs here
        //  UpdateCamera(&camera, CAMERA_ORBITAL);

        // 4. Render Cycle
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawModelEx(cube, (Vector3){0.0f, 0.0f, 0.0f}, // Position
                    (Vector3){1.0f, 0.0f, 0.0f},       // Rotation axis
                    rotation,                          // Rotation angle
                    (Vector3){1.0f, 1.0f, 1.0f},       // Scale
                    RED);
        EndMode3D();

        rotation += 1.0f;

        DrawText("Simple 3D Cube with Raylib", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // 5. Clean up
    UnloadModel(cube);
    CloseWindow();
    return 0;
}
