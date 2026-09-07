#pragma once
#include <functional>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

namespace DimEngineZ {

struct Vec3 {
private:
    Vector3 vec;

public:
    float& x;
    float& y;
    float& z;

    Vec3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
        : vec{x_, y_, z_}, x(vec.x), y(vec.y), z(vec.z) {}
    Vec3(const Vector3& initvec) : vec(initvec), x(vec.x), y(vec.y), z(vec.z) {}
    Vec3(std::initializer_list<float> lst)
        : vec{*(lst.begin()), *(lst.begin() + 1), *(lst.begin() + 2)}, x(vec.x), y(vec.y),
          z(vec.z) {}
    Vec3(const Vec3& other) : vec(other.vec), x(vec.x), y(vec.y), z(vec.z) {}

    float magnitude() const;
    Vector3 normalize() const;

    Vec3 operator+(const Vec3& other) {
        return Vec3(other.vec + vec);
    }
    Vec3 operator-(const Vec3& other) {
        return Vec3(other.vec - vec);
    }
    Vec3 operator*(const Vec3& other) {
        return Vec3(other.vec * vec);
    }
    Vec3 operator/(const Vec3& other) {
        return Vec3(other.vec / vec);
    }
    Vec3 operator*(float scalar) {
        return Vec3(vec * scalar);
    }
    Vec3 operator/(float scalar) {
        return Vec3(vec / scalar);
    }

    Vec3& operator=(const Vec3& other) {
        vec = other.vec;
        return *this;
    }
    Vec3 operator+=(const Vec3& other) {
        vec += other.vec;
        return *this;
    }

    operator const Vector3&() const {
        return vec;
    }
};
struct Transform {
    Vec3 position{0.0f, 0.0f, 0.0f};
    Vec3 rotation{0.0f, 0.0f, 0.0f};
    Vec3 scale{1.0f, 1.0f, 1.0f};

    float magnitude() const;
    float rotationMagnitude() const;

    void rotateX(float amount);
    void rotateY(float amount);
    void rotateZ(float amount);

    void move(const Vector3& displacement);
    void moveX(float amount);
    void moveY(float amount);
    void moveZ(float amount);

    void goTo(const Vector3& position);
    void goToX(float x);
    void goToY(float y);
    void goToZ(float z);

    Transform(Vector3 position_ = {0.0f, 0.0f, 0.0f}, Vector3 rotation_ = {0.0f, 0.0f, 0.0f},
              Vector3 scale_ = {1.0f, 1.0f, 1.0f})
        : position(position_), rotation(rotation_), scale(scale_) {}
};
struct DrawObject {
    Model model;
    Transform transform;
    Color color;

    DrawObject(const Mesh& mesh, const Transform& transform, Color color = RED)
        : model(LoadModelFromMesh(mesh)), transform(transform), color(color) {}

    ~DrawObject() {
        UnloadModel(model);
    }

    DrawObject(const DrawObject&) = delete;
    DrawObject& operator=(const DrawObject&) = delete;

    DrawObject(DrawObject&& other) noexcept
        : model(other.model), transform(other.transform), color(other.color) {
        other.model = {}; // to prevent other from unloading the same raylib resources
    }
    DrawObject& operator=(DrawObject&& other) noexcept {
        if (this != &other) {
            UnloadModel(model);

            model = other.model;
            transform = other.transform;
            color = other.color;

            other.model = {};
        }

        return *this;
    }

    void draw() const;
};
struct MovementObject {
    DrawObject shape;

    Vec3 velocity{0, 0, 0};
    float drag = 0.0f;
    float mass = 1.0f;

    Transform& transform() {
        return shape.transform;
    }

    const Transform& transform() const {
        return shape.transform;
    }

    MovementObject(DrawObject shape_) : shape(std::move(shape_)) {}

    void tick(float delta);
    void applyVelocity(const Vector3& applied);
    void setVelocity(const Vector3& newVelocity);
    void applyAcceleration(const Vector3& applied, float delta);
    void applyImpulse(const Vector3& impulse);
    void applyForce(const Vector3& force, float delta);
};
struct CollisionBox {
    BoundingBox box;
    void update(const Transform& transform);

    bool colliding_with(const CollisionBox& other) const;

    explicit CollisionBox(const BoundingBox& box_) : box(box_) {}
    explicit CollisionBox(const Model& model) : box(GetModelBoundingBox(model)) {}
};
struct PhysicsObject {
    CollisionBox collision;
    MovementObject core;
    bool is_on_ground = false;
    bool is_static = false;
    float bounce = 1.0f;

    void resolveCollision(PhysicsObject& other);
    void tick(float delta);

    PhysicsObject(DrawObject shape, float bounce_ = 1.0f)
        : collision(shape.model), core(std::move(shape)), bounce(bounce_) {}
};

struct Camera {
private:
    Camera3D cam;

public:
    Vector3& position;
    Vector3& up;
    Vector3 direction;
    float fovy = 90.0f;
    int& projection;

    Camera(const Vector3& position_, float fovy_, const Vector3& direction,
           int projection_ = CAMERA_PERSPECTIVE, const Vector3& up_ = Vector3{0.0f, 1.0f, 0.0f})
        : position(cam.position), up(cam.up), fovy(cam.fovy), projection(cam.projection),
          direction(direction) {
        cam.position = position_;
        cam.fovy = fovy_;
        cam.target = position_ + direction;
        cam.projection = projection_;
        cam.up = up_;
    }

    operator const Camera3D&() const {
        return cam;
    }

    void refreshTarget();

    void move(const Vector3& amount);
    void moveX(float amount);
    void moveY(float amount);
    void moveZ(float amount);

    void goTo(const Vector3& pos);
    void goToX(float x);
    void goToY(float y);
    void goToZ(float z);
};
} // namespace DimEngineZ
namespace DimEngineZ::manager {

void initWindow(int width, int height, const std::string& title);

using FuncitonCallback = std::function<bool()>;
int loop(int targetFPS, FuncitonCallback func);
int fixedloop(int targetFPS, float fixed_delta_inverse, std::function<bool(float)> func,
              FuncitonCallback render);
int fixedloop(int targetFPS, std::function<bool(float)> func, FuncitonCallback render);
bool render(Color background, bool clear, FuncitonCallback func);

} // namespace DimEngineZ::manager
namespace DimEngineZ::physics {
inline std::vector<PhysicsObject*> objects;
void registerObject(PhysicsObject* object);
void tick(float delta);
} // namespace DimEngineZ::physics