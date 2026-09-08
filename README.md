# DimEngineZ

> [!WARNING]
> **DimEngineZ is a work in progress.** APIs and architecture are likely to change as the project evolves. It is not meant to be production-ready.

**DimEngineZ** (pronounced *dimensions-E*) is a small, experimental game engine built on top of [Raylib](https://www.raylib.com/).

It is a personal project for exploring how game-engine systems can be designed and composed in C++. Note that DimEngineZ is not meant to be complete or production-ready: it is a place to experiment with the building blocks behind a simple game.  My goal is to keep those systems somewhat lightweight and composable. You can work directly with individual engine objects when you need control, or combine them into higher-level objects when that makes more sense.

## What it currently provides

DimEngineZ currently supports three main object types:

- **`DrawObject`**: handles drawing the object.
- **`MovementObject`**: handles movement-related functionality.
- **`PhysicsObject`**: combines the previous two shapes with physics simulation.

The physics system provides basic operations such as:
- `applyAcceleration()`
- `applyImpulse()`
- `applyForce()`
- velocity and movement handling
- static objects
- ground detection

The engine also provides a built-in application loop with a fixed update interval ("fixed delta"), allowing engine systems such as physics and rendering to be updated automatically.

## Example

The following creates a cube, places it above a ground plane, and gives it simple gravity and jumping:

```cpp
#include "../include/DimEngineZ.h"
#include "raylib.h"
#include <iostream>

int main() {
    namespace dez = DimEngineZ;

    dez::manager::initWindow(1000, 800, "3D Cube Renderer");

    dez::Camera camera({0.0f, 15.0f, 10.0f},
                       dez::CameraOptions{
                           .fovy = 90.0f,
                           .direction = {0.0f, -1.0f, -1.0f}
                       },
                       MAIN_CAMERA);

    camera.setTarget(Vector3{0.0f, 0.0f, 0.0f});

    // Create the player cube.
    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    auto draw = dez::DrawObject(mesh, dez::Transform(), GREEN);
    auto object = dez::PhysicsObject(std::move(draw), 0.5f);

    object.transform.position = Vector3{0.0f, 5.0f, 0.0f};

    // Create a static ground plane.
    mesh = GenMeshCube(20.0f, 1.0f, 20.0f);
    draw = dez::DrawObject(
        mesh,
        dez::Transform({0.0f, -0.5f, 0.0f}),
        BROWN
    );

    auto ground = dez::PhysicsObject(std::move(draw));
    ground.is_static = true;

    return dez::manager::main(60, [&](float delta) {
        constexpr float LAUNCH_FORCE = 50.0f;
        constexpr float GRAVITY = 9.81f;

        if (IsKeyPressed(KEY_SPACE) && object.is_on_ground) {
            object.core.applyImpulse(
                Vector3{0.0f, LAUNCH_FORCE, 0.0f}
            );
        }

        object.core.applyAcceleration(
            Vector3{0.0f, -GRAVITY, 0.0f},
            delta
        );

        if (IsKeyPressed(KEY_Q)) {
            return false;
        }

        camera.setTarget(object.transform.position);

        dez::manager::tick(delta);

        return true;
    });
}
```

## The object model

DimEngineZ is built around composing smaller pieces of functionality rather than putting everything into one large game-object abstraction. This is because I wanted to try out *composition* instead of *inheritance*, which I heard was better.

At the simplest level, a `DrawObject` can have a mesh, transform, and color:

```cpp
auto draw = dez::DrawObject(
    mesh,
    dez::Transform(),
    GREEN
);
```

A `PhysicsObject` can then be constructed from that `DrawObject`, taking its position:

```cpp
auto object = dez::PhysicsObject(
    std::move(draw),
    0.5f
);
```

This gives the object both its visual representation and physics-related state. The intention is that these systems remain useful independently while still being easy to combine when a game object needs multiple capabilities.

## Physics

Physics objects expose basic operations for manipulating their motion. For example, acceleration can be applied over a time step:

```cpp
object.core.applyAcceleration(
    Vector3{0.0f, -9.81f, 0.0f},
    delta
);
```

An impulse can be used for an instantaneous change in motion:

```cpp
object.core.applyImpulse(
    Vector3{0.0f, 50.0f, 0.0f}
);
```

This makes simple behaviors such as gravity and jumping straightforward to implement without requiring a large physics API. Static objects can also be created for things that should participate in the simulation without being moved by it:

```cpp
ground.is_static = true;
```

## The engine loop

DimEngineZ provides its own application loop through `manager::main()`.

A typical application supplies a target update rate and a callback:

```cpp
return dez::manager::main(60, [&](float delta) {
    // Game logic

    dez::manager::tick(delta);

    return true;
});
```
60 here is the target frame rate, which also calculates the fixed delta, which the callback receives as `delta`, allowing game logic to operate using the elapsed timestep. 
`manager::tick(delta)` then advances the engine's managed systems, including physics and drawing. The main loop can provide a predictable update order and timing rather than requiring every application to build its own loop and timing infrastructure.

## Cameras

DimEngineZ also provides a camera abstraction around Raylib's 3D camera functionality.

```cpp
dez::Camera camera(
    {0.0f, 15.0f, 10.0f},
    dez::CameraOptions{
        .fovy = 90.0f,
        .direction = {0.0f, -1.0f, -1.0f}
    },
    MAIN_CAMERA
);
```

Cameras can then be updated as part of the game loop. For example, the camera in the example follows the physics object:

```cpp
camera.setTarget(object.transform.position);
```
Or by changing its direction. Note that `transform.position` should **not** be updated manually; it is preferred to use `move`, `moveX`, etc. or `goTo`, `goToX`, etc. to move, as those update the camera's `direction`.

