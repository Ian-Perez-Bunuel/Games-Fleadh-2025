#pragma once

#include "raylib.h"
#include "stdio.h"

#include "../include/ParticleSpawner.h"
class Player;

class Projectile
{
public:
    Projectile(Model& t_model, Color t_color, Vector3 t_pos, Vector3 t_targetPos, Player& t_player);
    void draw();
    void drawParticles();
    void update();
    void explosion();

    bool isExploding() { return explode; }
    bool isActive() { return active; }
    
private:
    Model& model;
    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;

    void moveToTarget();
    void rotateToTarget();

    bool active = false;
    bool explode = false;
    float explosionRadius = 65.0f;
    const float EXPLOSION_DURATION = 1.0f;
    float explosionTimer = 0.0f;
    ParticleSpawner particleSpawner;

    Vector3 position;
    float radius = 0.1f;

    Vector3 targetPos;
    Vector3 axis;
    float angle;

    Player& player;

    Color color;

    Sound explosionSound;
    bool explosionPlayed = false;

    // Normalize to -1 to 1 range
    float normalizeSigned(float x, float t_min, float t_max) { return 2 * (x - t_min) / (t_max - t_min) - 1; }
    Vector2 convertToGameCoords(Vector3 t_originalCoords);

    // Dot product function
    double dot(Vector3 a, Vector3 b) const { return a.x * b.x + a.y * b.y + a.z * b.z; }
    // Magnitude (length) of the vector
    double magnitude(Vector3 a) const { return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
    // Cross product function
    Vector3 cross(Vector3 a, Vector3 b) const { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
    float angleBetweenAtan2(Vector3 t_a, Vector3 t_b);
};

#include "../include/Player.h"