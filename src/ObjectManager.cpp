#include "../include/ObjectManager.h"
#include "../include/Globals.h"

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
        if (distToObject < lowestDist && !object->checkGrabbed())
        {
            lowestDist = distToObject;
            closestObject = object;
        }
    }

    return closestObject;
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
}