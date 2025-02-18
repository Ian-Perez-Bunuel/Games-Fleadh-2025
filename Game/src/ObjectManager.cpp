#include "../include/ObjectManager.h"
#include "../include/Globals.h"


const int ObjectManager::SMALL = 15;
const int ObjectManager::MEDIUM = 25;
const int ObjectManager::LARGE = 45;


void ObjectManager::setObjects()
{
    texture = LoadTexture("resources/Art/asteroidWhite.png");

    objects.clear();

    for (int i = 0; i < OBJECT_AMOUNT; i++)
    {
        Vector2 position = {(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)};
        int randSize = rand() % 3;
        switch (randSize)
        {
        case 0:
            objects.push_back(std::make_shared<Object>(texture, position, SMALL));
            break;
        case 1:
            objects.push_back(std::make_shared<Object>(texture, position,  MEDIUM));
            break;
        case 2:
            objects.push_back(std::make_shared<Object>(texture, position, LARGE));
            break;
        }
    }
}

std::shared_ptr<Object> ObjectManager::findClosestToPlayer(Player t_player)
{
    float lowestDist = 100000.0f;
    std::shared_ptr<Object> closestObject = nullptr;

    for (std::shared_ptr<Object>& object : objects)
    {
        float distToObject = pointToPointDist(object->getPos(), t_player.getPos());
        if (distToObject < lowestDist && !object->checkGrabbed() && object->isActive())
        {
            lowestDist = distToObject;
            closestObject = object;
        }
    }

    return closestObject;
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
    
    splitObject();
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
}

void ObjectManager::update()
{
    for (std::shared_ptr<Object>& object : objects)
    {
        object->update();

    }
    checkCollisions();
}