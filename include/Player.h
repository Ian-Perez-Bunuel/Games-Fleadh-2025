#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stdio.h"
#include <math.h>

class Player
{
public:
    Player();

    void move();
    void draw();

private:
    const int RADIUS = 20;

    Vector2 position;

    void screenWrapping();

    // Movement
    void capSpeed();
    void dampingMovement();
    const float SPEED = 0.25;
    const int MAX_SPEED = 8;
    Vector2 acceleration = {0.0f, 0.0f};
    Vector2 velocity;
    const float FRICTION = 0.5f;


};

#endif // PLAYER_H