#include "../include/Projectile.h"
#include <cmath>

#include "../include/Globals.h"

Projectile::Projectile(Vector3 t_pos, Vector3 t_targetPos)
    : position(t_pos), targetPos(t_targetPos)
{
    active = true;
}

void Projectile::draw()
{
    if (active)
    {
        DrawSphere(position, radius, RED);
    }
    else if (explode)
    {
        
    }
}

void Projectile::update()
{
    if (active)
    {
        moveToTarget();
    }
    else if (explode)
    {
        hit();
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

void Projectile::hit()
{
    
}