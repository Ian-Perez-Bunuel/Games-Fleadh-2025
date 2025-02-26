#include "../include/ObjectManager.h"
#include "../include/Globals.h"
#include <bits/stdc++.h>


const int ObjectManager::SMALL = 15;
const int ObjectManager::MEDIUM = 25;
const int ObjectManager::LARGE = 45;

ObjectManager::ObjectManager(Player& t_player) : player(t_player)
{
    texture1 = LoadTexture("resources/Art/2D/object1.png");
    texture2 = LoadTexture("resources/Art/2D/object2.png");
    texture3 = LoadTexture("resources/Art/2D/object3.png");
    texture4 = LoadTexture("resources/Art/2D/object4.png");

    initPickupTextures();
}

void ObjectManager::initPickupTextures()
{
    healthPackTexture = LoadTexture("resources/Art/2D/heal.png");
}

void ObjectManager::setObjects()
{
    objects.clear();
    pickups.clear();

    for (int i = 0; i < OBJECT_MIN; i++)
    {
        addObject();
    }
}

void ObjectManager::addObject()
{
    setRandomTexture();
    // Vector2 position = {-(rand() % 100) - LARGE, (float)(rand() % SCREEN_HEIGHT)}; // Spawns them all on the left side of the screen
    Vector2 position = genOffScreenPos();
    float randDir = atan2f((SCREEN_WIDTH / 2) - position.x, (SCREEN_HEIGHT / 2) - position.y) * RAD2DEG;

    printf("\n\nPOS: %f, %f", position.x, position.y);
    printf("\nDIRECTION: %f\n\n", randDir);

    int randSize = rand() % 3;
    switch (randSize)
    {
    case 0:
        objects.push_back(std::make_shared<Object>(texture, position, MEDIUM, randDir));
        break;
    case 1:
        objects.push_back(std::make_shared<Object>(texture, position,  MEDIUM, randDir));
        break;
    case 2:
        objects.push_back(std::make_shared<Object>(texture, position, LARGE, randDir));
        checkForPickup(objects[objects.size() - 1]);  
        break;
    }
}

Vector2 ObjectManager::genOffScreenPos()
{
    Vector2 pos;
    if (rand() % 2 == 0)
    {
        pos.x = -(rand() % 10) - LARGE;
    }
    else
    {
        pos.x = (rand() % SCREEN_WIDTH + LARGE) + SCREEN_WIDTH;
    }

    if (rand() % 2 == 0)
    {
        pos.y = -(rand() % 10) - LARGE;
    }
    else
    {
        pos.y = (rand() % SCREEN_HEIGHT + LARGE) + SCREEN_HEIGHT;
    }

    return pos;
}

void ObjectManager::setRandomTexture()
{
    int randNum = rand() % 4;

    switch (randNum)
    {
    case 0:
        texture = texture1;
        break;

    case 1:
        texture = texture2;
        break;

    case 2:
        texture = texture3;
        break;

    case 3:
        texture = texture4;
        break;
    }
}

void ObjectManager::checkForPickup(std::shared_ptr<Object> t_object)
{
    int pickupChance = rand() % PICKUP_CHANCE;
    if (pickupChance == 0)
    {
        // Make a switch statement if multiple pickups in the future
        pickups.push_back(std::make_shared<HealthPack>(healthPackTexture, player, t_object, 20));
    }
}

std::shared_ptr<Object> ObjectManager::findClosestToPlayer()
{
    float lowestDist = 100000.0f;
    std::shared_ptr<Object> closestObject = nullptr;

    for (std::shared_ptr<Object>& object : objects)
    {
        float distToObject = pointToPointDist(object->getPos(), player.getPos());
        if (distToObject < lowestDist && !object->checkGrabbed() && object->isActive() && !object->checkToPlanet())
        {
            lowestDist = distToObject;
            closestObject = object;
        }
    }

    return closestObject;
}

void ObjectManager::checkPlayerCollisions(std::shared_ptr<Object>& t_object)
{
    if (!t_object->checkGrabbed() && t_object->isActive() && t_object->checkCollidable() && !t_object->checkIfPickedUp())
    {
        float dist = pointToPointDist(player.getPos(), t_object->getPos());

        if (dist <= player.getRadius() + t_object->getRadius())
        {
            // Collision occurred
            player.takeDamage(t_object->getRadius() / 2);
        }
    }
}

void ObjectManager::checkCollisions()
{
    for (std::shared_ptr<Object>& currentObject : objects)
    {
        // If the object is grabbed skip the rest of this loop
        if (currentObject->checkGrabbed() || !currentObject->isActive() || !currentObject->checkCollidable())
        {
            continue;
        }

        for (std::shared_ptr<Object>& otherObject : objects)
        {
            // If the object is grabbed skip the rest of the function
            if (otherObject->checkGrabbed() || !otherObject->isActive() || !otherObject->checkCollidable())
            {
                continue;
            }

            // If its not the same object
            if (currentObject != otherObject)
            {
                if (currentObject->checkObjectCollisions(otherObject))
                {
                    objectsToSplit.push_back(currentObject);
                    objectsToSplit.push_back(otherObject);
                    
                    currentObject->destroy();
                    otherObject->destroy();
                }
            }
        }
    }

    // Split objects that need splitting
    splitObject();

    removeNotActives();          
}

void ObjectManager::keepObjectsAboveMin()
{
    int objectAmountNeeded = 0;
    if (objects.size() < OBJECT_MIN)
    {
        objectAmountNeeded = OBJECT_MIN - objects.size();

        for (int i = 0; i < objectAmountNeeded; i++)
        {
            addObject();
        }   
    }
}

void ObjectManager::removeNotActives()
{
    // Remove all objects that are marked as not active (Including their particles).
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                   [](const std::shared_ptr<Object>& obj) {
                       return obj->isParticalsActive() && !obj->isActive();
                   }),
                   objects.end());
}

void ObjectManager::splitObject()
{
    if (objectsToSplit.size() > 0)
    {
        for (std::weak_ptr<Object>& splittingObject : objectsToSplit)
        {
            int randAngleOffset = (rand() % 20) - 10;

            switch (splittingObject.lock()->getRadius())
            {
                case SMALL:
                    break;
                
                case MEDIUM:
                for (int i = 0; i < 4; i++)
                {
                    objects.push_back(std::make_shared<Object>(texture, splittingObject.lock()->getPos(), SMALL, i * (90 + randAngleOffset)));
                }
                break;

            case LARGE:
                for (int i = 0; i < 4; i++)
                {
                    objects.push_back(std::make_shared<Object>(texture, splittingObject.lock()->getPos(), MEDIUM, i * (90 + randAngleOffset)));
                }
                break;
    
            default:
            break;
            }
        }
    }

    objectsToSplit.clear();
}



void ObjectManager::draw()
{
    for (std::shared_ptr<Object>& object : objects)
    {
        object->draw();
    }

    for (std::shared_ptr<PickUp>& pickup : pickups)
    {
        pickup->draw();
    }
}

void ObjectManager::update(Planet& t_planet)
{
    for (std::shared_ptr<Object>& object : objects)
    {
        object->update();

        if (object->checkToPlanet())
	    {
		    object->movementToPlanet(t_planet);
	    }

        checkPlayerCollisions(object);
    }
    checkCollisions();

    for (std::shared_ptr<PickUp>& pickup : pickups)
    {
        pickup->update();
    }

    keepObjectsAboveMin();
}