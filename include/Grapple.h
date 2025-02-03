#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>
#include <vector>

#include "../include/Object.h"

class Grapple
{
public:
    void shoot(std::shared_ptr<Object> t_target, Vector2& t_userPos);
    void release(Vector2 t_releaseDir);
    void update();
    void draw();

    bool isActive() { return active; }

private:
    bool active = false;

    std::vector<std::shared_ptr<Object>> grappledObjects;
    Vector2* userPos;

    const int MAX_LENGTH = 250;
    float length = 0.0f;
    Color color = GREEN;

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};