#include "raylib.h"
#include "stdio.h"
#include <math.h>


class Object
{
public:
    Object(Vector2 t_pos = {0.0, 0.0});

    void update();
    void draw();

    Vector2 getPos() { return position; }
    int getRadius() { return RADIUS; }

    void grab(Vector2 t_anchorPos);
    void held(Vector2 t_anchorPos, float t_dist);
    void released();

private:
    const int RADIUS = 20;

    bool grabbed = false;
    float angle = 0.0f;

    Vector2 position;

    // Physics
    float mass = 10;
    float speed = 4.0f;
    float rotationSpeed = 450;
    Vector2 velocity;

    // Aim line
    int lineLength = 150;
};