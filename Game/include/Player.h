#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/Grapple.h"
class Object;

class Player
{
public:
    Player();
    void initialize();

    void update(Vector2 t_stickDir, Vector2 t_cursorPos);
    void draw();
    void draw3D();

    Vector2& getPos() { return position; }
    int getRadius() { return RADIUS; }

    // Grapple commands
    void shootGrapple(std::shared_ptr<Object> t_object);
    void releaseGrapple(Vector2 t_releaseDir, bool t_toPlanet);

    void heal(int t_amount);
    void takeDamage(int t_amount);

    static void increase3DStage() { stage++; }

private:
    const int RADIUS = 40;
    Texture2D texture;
    Color color = WHITE;

    // 3D
    void updateModels();
    Vector3 position3D;
    Model ring;
    Model beams;
    Model hull;
    static int stage;
    const int tilt = 90;
    
    Vector2 position;

    void animation();
    bool expand = false;
    float scale = 1.0f;

    // Mouse Rotation
    void rotateToMouse();
    void rotateToController(Vector2 t_cursorPos);
    float rotation = 0;

    // Movement
    void move();
    void boundryChecking();
    void controllerMovement(Vector2 t_stickDir, Vector2 t_cursorPos);
    void capSpeed();
    const float SPEED = 0.25;
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;

    // Grapples
    void setGrapplePos();
    void setGrappleAgression();
    static const int GRAPPLE_AMOUNT = 8;
    Grapple grapples[GRAPPLE_AMOUNT];
    
    bool alive = true;

    // Health
    void kill();
    bool lastHit = false;
    const int MAX_HEALTH = 100;
    int health = MAX_HEALTH;
    // Invincible
    void invincibleClock();
    const float INVINCIBILITY_DURATION = 0.5f; // 0.5 seconds
    float invincibilityTimer = 0;
    bool invincible = false;

    // Health bar
    void initHealthBar();
    void drawHealthBar();
    Vector2 barPos;
    int barHeight = 25;
    int barLength = 0;

    // Achivement trackers
    int objectsGrabbed = 0;

    Sound damageSound;

    // Normalize to -1 to 1 range
    float normalizeSigned(float x, float t_min, float t_max) { return 2 * (x - t_min) / (t_max - t_min) - 1; }
    Vector3 convertToMiddleCoords(Vector2 t_originalCoords);
    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};
