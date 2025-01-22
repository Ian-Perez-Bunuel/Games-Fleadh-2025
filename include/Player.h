#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stdio.h"
#include <math.h>

class Player
{
public:
    Player();

    void update();
    void draw();

private:
    const int RADIUS = 20;

    Vector2 position;

    void screenWrapping();

    // Grapple
    void shootGrapple();
    void releaseGrapple();
    bool grappleActive = false;
    Vector2 grappleAnchor;

    // Movement
    void move();
    void capSpeed();
    const float SPEED = 0.25;
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;


};

#endif // PLAYER_H