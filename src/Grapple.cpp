#include "../include/Grapple.h"

void Grapple::shoot(std::shared_ptr<Object> t_target, Vector2& t_userPos)
{
    userPos = &t_userPos;

    float distToTarget = pointToPointDist(*userPos, t_target->getPos());

    if (distToTarget < MAX_LENGTH)
    {
        active = true;
        grappledObject = t_target; 
        grappledObject->grab(*userPos);

        // Set distance of target grappled
        length = distToTarget;
    }
}

void Grapple::release()
{
    grappledObject->released();
    active = false;
    color = GREEN;
}

void Grapple::update()
{
    grappledObject->held(*userPos, length);
}
void Grapple::draw()
{
    if (active)
    {
        DrawLineV(*userPos, grappledObject->getPos(), color);
    }
}