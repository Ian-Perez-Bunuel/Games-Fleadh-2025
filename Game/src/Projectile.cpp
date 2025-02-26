#include "../include/Projectile.h"
#include <cmath>

#include "../include/Globals.h"

Projectile::Projectile(Vector3 t_pos, Vector3 t_targetPos)
    : position(t_pos), targetPos(t_targetPos)
{
    active = true;

    model = LoadModel("resources/Art/3D/missile.obj");
}

void Projectile::draw()
{
    if (active)
    {
        DrawModel(model, position, 0.1f, RED);
    }
}

void Projectile::update()
{
    if (active)
    {
        moveToTarget();
    }
}

void Projectile::moveToTarget()
{
    // Compute the direction vector (pointing toward the target)
    Vector3 direction = {
        targetPos.x - position.x,
        targetPos.y - position.y,
        targetPos.z - position.z
    };

    // Calculate the length (magnitude) of the direction vector
    float length = sqrtf(direction.x * direction.x + 
                         direction.y * direction.y + 
                         direction.z * direction.z);

    // Normalize the direction vector
    if (length > 0.0001f) 
    {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }   

    // Move projectile
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    if (position.z >= targetPos.z)
    {
        active = false;
        explode = true;
    }
}

void Projectile::explosion(Vector3 t_playerPos)
{
    float dist = sqrtf(pow(t_playerPos.x - position.x, 2) + pow(t_playerPos.y - position.y, 2) + pow(t_playerPos.z - position.z, 2));
    if (dist <= 33 + radius); // 33 is the player's radius
    {

    }
}