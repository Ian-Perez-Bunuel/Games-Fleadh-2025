#pragma once

#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/ParticleSpawner.h"


class Planet;


class Object
{
public:
    Object(Sound& t_breakSound, Sound& t_grabbedSound, Texture2D& t_texture, Vector2 t_pos = {0.0f, 0.0f}, int t_size = 10, int dirAngle = 35);

    virtual void update();
    virtual void draw();

    Vector2 getPos() { return position; }
    int getRadius() { return radius; }
    bool checkGrabbed() { return grabbed; }
    bool isActive() { return active; }
    bool isParticalsActive() { return particleSpawner.checkIfParticalsActive(); }
    virtual bool checkCollidable() { return collidable; }
    bool checkToPlanet() { return toPlanet; }
    bool checkIfPickedUp() { return beenPickedUp; }
    
    void grab();
    void held(Vector2 t_anchorPos, float t_dist);
    virtual void released(Vector2 t_releaseDir, bool t_toPlanet);

    bool checkObjectCollisions(std::shared_ptr<Object> t_otherObject);
    void destroy();
    void deactivate() { active = false; }

    // Planet things
    virtual void movementToPlanet(Planet& t_planet);

    // Achievement
    bool getRotationMaxHit() { return hitMaxRotationSpeed; }
    
protected:
    void move(); 

    bool active = false;
    bool beenPickedUp = false;

    int radius = 20;
    Texture2D texture;
    Color color;

    float scale = 1.0f;

    Vector2 position = {0.0f, 0.0f};
    Vector2 velocity;

    bool grabbed = false;
    // Movement towards planet
    Vector2 planetPos;
    bool toPlanet = false;

    float rotationSpeed = 150.0f;
    const int MAX_ROTATION_SPEED = 260;

    bool correctDist = false;

    // Particals
    ParticleSpawner particleSpawner;

private:   
    bool moveToRotationArea(Vector2 t_anchorPos, float t_targetDist);

    void checkForSpeedIncrease();
    void invinsabilityCheck();

    void loop();

    // Invinsability
    bool collidable = false;
    const int INVINCIBILITY_DURATION = 60 * 0.5;
    int invincibilityTimer = 0;

    float angle = -1.0f;

    // Physics
    float mass = 10;
    float speed = 2.0f;
    float anchorDist = -1.0f;


    // Sounds
    Sound& breakSound;
    Sound& grabbedSound;

    // Achievement Tracking
    bool hitMaxRotationSpeed = false;

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
    float radiansToDegrees(float t_radians) { return t_radians * (180/3.14f); }
    float degreesToRadians(float t_degrees) { return t_degrees * (3.14f / 180); }
};