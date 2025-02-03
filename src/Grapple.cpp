#include "../include/Grapple.h"

void Grapple::shoot(std::shared_ptr<Object> t_target, Vector2& t_userPos)
{
    userPos = &t_userPos;

    float distToTarget = pointToPointDist(*userPos, t_target->getPos());

    if (distToTarget < MAX_LENGTH)
    {
        active = true;

        t_target->grab(*userPos);

        grappledObjects.push_back(t_target); 
    }
}

void Grapple::release(Vector2 t_releaseDir)
{
    for (std::shared_ptr<Object>& object : grappledObjects)
    {
        object->released(t_releaseDir);
    }
    grappledObjects.clear();
    active = false;
    color = GREEN;
}

void Grapple::update()
{
    for (std::shared_ptr<Object>& object : grappledObjects)
    {
        float distToTarget = pointToPointDist(*userPos, object->getPos());
        object->held(*userPos, distToTarget);
    }
}
void Grapple::draw()
{
    if (active)
    {
        for (std::shared_ptr<Object>& object : grappledObjects)
        {
            DrawLineV(*userPos, object->getPos(), color);
        }
    }
}