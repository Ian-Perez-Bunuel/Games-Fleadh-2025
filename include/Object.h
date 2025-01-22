#include "raylib.h"
#include "stdio.h"

class Object
{
public:
    Object(Vector2 t_pos = {0.0, 0.0});

    void update();
    void draw();

private:
    const int RADIUS = 20;

    Vector2 position;
    Vector2 velocity;
};