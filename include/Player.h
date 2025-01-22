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
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;


};

#endif // PLAYER_H