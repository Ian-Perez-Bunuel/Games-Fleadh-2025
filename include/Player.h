#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/Object.h"

class Player
{
public:
    Player();

    void update(Vector2 t_stickDir);
    void draw();

    // Grapple
    bool isGrappleActive() { return grappleActive; }
    void shootGrapple(std::shared_ptr<Object> t_target);
    void releaseGrapple();

private:
    const int RADIUS = 20;

    Vector2 position;

    void screenWrapping();

    // Grapple
    void updateGrapple();
    bool grappleActive = false;
    float grappleDist = 0;
    const int MAX_GRAPPLE_DIST = 250;
    std::shared_ptr<Object> grappledObject;
    // Color
    Color grappleColor = GREEN;

    // Movement
    void move();
    void controllerMovement(Vector2 t_stickDir);
    void capSpeed();
    const float SPEED = 0.25;
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};

#endif // PLAYER_H