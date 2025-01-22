#include "../include/Planet.h"
#include "../include/Globals.h"

Planet::Planet(Vector2 t_pos) : position(t_pos)
{
}

void Planet::draw()
{
    DrawCircleLinesV(position, gravityRange, PURPLE);
    DrawCircleV(position, radius, BLUE);
}