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
    }

    collisionDetection();

    particleSpawner.update();
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
