#include "../include/Projectile.h"
#include "../include/Globals.h"
#include "stdio.h"

#include <random>

Projectile::Projectile(Vector2 t_pos) : position(t_pos)
{
    Vector2 randPos = {(float)(rand() % SCREEN_SIZE), (float)(rand() % SCREEN_SIZE)};
    velocity = dirVector(position, randPos);
}

void Projectile::draw()
{
    DrawCircleV(position, radius, RED);
}

void Projectile::move()
{
    position.x += velocity.x / 100;
    position.y += velocity.y / 100;
}

void Projectile::moveToPlanet(float t_gravity, Vector2 t_planetPos)
{
    velocity.x += (dirVector(position, t_planetPos).x / 250) * t_gravity;
    velocity.x += (dirVector(position, t_planetPos).y / 250) * t_gravity;
}