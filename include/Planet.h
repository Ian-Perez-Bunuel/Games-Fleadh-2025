#include "raylib.h"

class Planet
{
public:
    Planet(Vector2 t_pos);

    void draw();

    int getGravityRadius() { return gravityRange; }
    Vector2 getPos() { return position; }

    int getGravity() { return gravityStrength; }

private:
    int radius = 15;

    int gravityRange = 75;
    int gravityStrength = 50;

    Vector2 position = {200, 200};

};