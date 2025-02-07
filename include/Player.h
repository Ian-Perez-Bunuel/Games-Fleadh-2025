#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/Grapple.h"

class Player
{
public:
    Player();
    void initialize();

    void update(Vector2 t_stickDir);
    void draw();

    Vector2& getPos() { return position; }

    // Grapple commands
    void shootGrapple(std::shared_ptr<Object> t_target);
    void releaseGrapple(Vector2 t_releaseDir);

private:
    const int RADIUS = 30;
    Texture2D texture;

    Vector2 position;

    void screenWrapping();

    // Movement
    void move();
    void controllerMovement(Vector2 t_stickDir);
    void capSpeed();
    const float SPEED = 0.25;
    const int MAX_SPEED = 6;
    Vector2 velocity;
    const float FRICTION = 0.9f;

    static const int GRAPPLE_AMOUNT = 8;
    Grapple grapples[GRAPPLE_AMOUNT];

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};

#endif // PLAYER_H