#include "raylib.h"
#include "stdio.h"
#include <math.h>


class Object
{
public:
    Object(Vector2 t_pos = {0.0, 0.0});

    void update(Vector2 t_stickDir);
    void draw();

    Vector2 getPos() { return position; }
    int getRadius() { return RADIUS; }

    void dragging(Vector2 t_anchorPos, float t_dist);
    void stickControl(Vector2 t_stickDir);
    void released();

private:
    const int RADIUS = 20;

    Vector2 position;

    // Physics
    float mass = 10;
    float speed = 3.5f;
    Vector2 velocity;


    float vectorLenght(Vector2 t_a, Vector2 t_b) { return (sqrt((t_b.x - t_a.x) * (t_b.x - t_a.x) + (t_b.y - t_a.y) * (t_b.y - t_a.y))); }
    Vector2 scaleVectorLenght(Vector2 t_startPoint, Vector2 t_endPoint, int t_distance);
};