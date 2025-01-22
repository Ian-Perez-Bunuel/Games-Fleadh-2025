#include "../include/Object.h"

Object::Object(Vector2 t_pos) : position(t_pos)
{
}

void Object::update()
{
    position.x += velocity.x;
    position.y += velocity.y;

    velocity.x *= 0.9;
    velocity.y *= 0.9;
}

void Object::draw()
{
    DrawCircleV(position, RADIUS, RED);
}
