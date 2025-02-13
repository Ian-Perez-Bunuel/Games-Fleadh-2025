#include "../include/ObjectManager.h"
#include "../include/Globals.h"

const int ObjectManager::SMALL = 15;
const int ObjectManager::MEDIUM = 25;
const int ObjectManager::LARGE = 45;


void ObjectManager::setObjects()
{
    objects.clear();

    for (int i = 0; i < 50; i++)
    {
        Vector2 position = {(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)};
        int randSize = rand() % 3;
        switch (randSize)
        {
        case 0:
            objects.push_back(std::make_shared<Object>(position, SMALL));
            break;
        case 1:
            objects.push_back(std::make_shared<Object>(position, MEDIUM));
            break;
        case 2:
            objects.push_back(std::make_shared<Object>(position, LARGE));
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
    for (std::size_t currentObject = 0; currentObject < objects.size(); currentObject++)
    {
        // If the object is grabbed skip the rest of this loop
        if (objects[currentObject]->checkGrabbed() || !objects[currentObject]->isActive())
        {
            continue;
        }

        for (std::size_t otherObject = 0; otherObject < objects.size(); otherObject++)
        {
            // If the object is grabbed skip the rest of the function
            if (objects[otherObject]->checkGrabbed() || !objects[otherObject]->isActive())
            {
                continue;
            }

            // If its not the same object
            if (currentObject != otherObject)
            {
                if (objects[currentObject]->checkObjectCollisions(objects[otherObject]))
                {
                    splitObject(objects[currentObject]);
                    splitObject(objects[otherObject]);

                    objects[currentObject]->destroy();
                    objects[otherObject]->destroy();
                }
            }
        }
    }
}

void ObjectManager::splitObject(std::shared_ptr<Object> t_destroyedObject)
{
    printf("SPLIT");
    switch (t_destroyedObject->getRadius())
    {
    case SMALL:
        break;

    case MEDIUM:
        for (int i = 0; i < 4; i++)
        {
            objects.push_back(std::make_shared<Object>(t_destroyedObject->getPos(), SMALL, i * 90));
        }
        break;

    case LARGE:
        for (int i = 0; i < 4; i++)
        {
            objects.push_back(std::make_shared<Object>(t_destroyedObject->getPos(), MEDIUM, i * 90));
        }
        break;
    
    default:
        break;
    }
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

        checkCollisions();
    }
}