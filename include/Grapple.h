#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>
#include <vector>

#include "../include/Object.h"

struct State
{
    int frequency;
    int amplitude;
    int speed;
    int thickness;
};


class Grapple
{
public:
    Grapple();
    void setStartPos(Vector2 t_startPos, Vector2& t_userPos);

    void shoot(std::shared_ptr<Object> t_target);
    void release(Vector2 t_releaseDir);
    void update();
    void draw();

    bool isActive() { return active; }
    Vector2 getStartPoint() { return  startPos; }

private:
    bool active = false;
    Vector2 startPos; // The point where the spline comes out of

    std::shared_ptr<Object> grappledObject;
    Vector2* userPos;

    const int MAX_LENGTH = 400;
    const int IDLE_LENGTH = 120;
    float length = 0.0f;

    // Spline
    void updateSpline();
    static const int NUM_POINTS = 6;
    Vector2 points[NUM_POINTS];

    State states[3];
    int currentState = 0;

    Color color = YELLOW;

    void aimTimer();
    bool aiming = false;
    const int AIM_DURATION = 60 * 0.2; // 0.2 seconds
    int aimClock = 0;

    float degreesToRadians(float t_degrees) { return t_degrees * (3.14f / 180); }
    float radiansToDegrees(float t_radians) { return t_radians * (180/3.14f); }
    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
    Vector2 scaleVectorLenght(Vector2 t_startPoint, Vector2 t_endPoint,  int t_distance);
};