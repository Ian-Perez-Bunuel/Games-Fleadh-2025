#pragma once

#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/ParticleSpawner.h"


class Object
{
public:
    Object(Texture2D& t_texture, Vector2 t_pos = {0.0f, 0.0f}, int t_size = 10, int dirAngle = -30);

    void update();
    void draw();

    Vector2 getPos() { return position; }
    int getRadius() { return radius; }
    bool checkGrabbed() { return grabbed; }
    bool isActive() { return active; }
    bool checkCollidable() { return collidable; }
    
    void grab();
    void held(Vector2 t_anchorPos, float t_dist);
    void released(Vector2 t_releaseDir);

    bool checkObjectCollisions(std::shared_ptr<Object> t_otherObject);
    void destroy();

    
private:
    bool moveToRotationArea(Vector2 t_anchorPos, float t_targetDist);
    
    void checkForSpeedIncrease();
    void invinsabilityCheck();

    
    void loop();
    
    bool active = false;

    // Invinsability
    bool collidable = false;
    const int INVINCIBILITY_DURATION = 60 * 0.5;
    int invincibilityTimer = 0;

    int radius = 20;
    Texture2D texture;

    bool grabbed = false;
    bool correctDist = false;
    float angle = -1.0f;

    Vector2 position = {0.0f, 0.0f};

    // Physics
    float mass = 10;
    float speed = 2.0f;
    float rotationSpeed = 150.0f;
    const int MAX_ROTATION_SPEED = 260;
    Vector2 velocity;
    float anchorDist = -1.0f;

    Color color;

    // Particals
    ParticleSpawner particleSpawner;

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
    float radiansToDegrees(float t_radians) { return t_radians * (180/3.14f); }
    float degreesToRadians(float t_degrees) { return t_degrees * (3.14f / 180); }
};