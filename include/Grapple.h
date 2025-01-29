#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>

#include "../include/Object.h"

class Grapple
{
public:
    void shoot(std::shared_ptr<Object> t_target, Vector2& t_userPos);
    void release();
    void update();
    void draw();

    bool isActive() { return active; }

private:
    bool active = false;

    std::shared_ptr<Object> grappledObject;
    Vector2* userPos;

    float length = 0;
    const int MAX_LENGTH = 400;
    Color color = GREEN;

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};