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

void ObjectManager::addObject()
{
    setRandomTexture();
    Vector2 position = genSpawnPos();

    // Have multiple spawnpoint positions with a queue

    int randSize = rand() % 3;
    switch (randSize)
    {
    case 0:
        objects.push_back(std::make_shared<Object>(texture, position, MEDIUM));
        break;
    case 1:
        objects.push_back(std::make_shared<Object>(texture, position,  MEDIUM));
        break;
    case 2:
        objects.push_back(std::make_shared<Object>(texture, position, LARGE));
        checkForPickup(objects[objects.size() - 1]);  
        break;
    }
}

Vector2 ObjectManager::genSpawnPos()
{
    Vector2 start = { -(SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 };
    Vector2 end = { (SCREEN_WIDTH * 0.75f), -(SCREEN_HEIGHT * 0.75f)};

    // Pick a random t in [0, 1]
    float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);  // yields a float between 0 and 1

    // Lerp between start and end
    Vector2 spawnPos;
    spawnPos.x = start.x + t * (end.x - start.x);
    spawnPos.y = start.y + t * (end.y - start.y);

    return spawnPos;
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
    if (objects.size() < OBJECT_MIN)
    {
        if (queueTimer < SPAWN_WAIT)
        {
            queueTimer += GetFrameTime();
        }
        else
        {
            for (int i = 0; i < AMOUNT_TO_SPAWN; i++)
            {
                addObject();
            }

            queueTimer = 0.0f;
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