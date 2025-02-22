#include "../include/PickUp.h"

void PickUp::update()
{
    if (parentObject == nullptr || (parentObject->isActive() && !parentObject->checkToPlanet()))
    {
        position = parentObject->getPos();
    }
    else
    {
        if (!active)
        {
            active = true;
        }

        if (!pickedUp)
        {
            animate();
        }
    }

    collisionDetection();

    particleSpawner.update();
}

void PickUp::animate()
{
    if (abs(offsetY) > abs(MAX_OFFSET))
    {
        animationSpeed *= -1;

        if (animationSpeed > 0)
        {
            particleSpawner.setValues(position, 360, 0);
            particleSpawner.spawn();
        }
    }

    offsetY += animationSpeed;
    position.y += offsetY;
}

void PickUp::collisionDetection()
{
    if (active && !pickedUp)
    {
        float dist = pointToPointDist(player.getPos(), position);

        if (dist <= player.getRadius() + radius)
        {
            // Collision occurred
            onPickup();

            pickedUp = true;
        }
    }
}
