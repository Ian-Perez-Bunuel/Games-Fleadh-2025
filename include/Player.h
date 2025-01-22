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

    void update();
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
    std::shared_ptr<Object> grappledObject;

    // Movement
    void move();
    void capSpeed();
    const float SPEED = 0.25;
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;


};

#endif // PLAYER_H