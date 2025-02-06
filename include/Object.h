#include "raylib.h"
#include "stdio.h"
#include <math.h>


class Object
{
public:
    Object(Vector2 t_pos = {0.0f, 0.0f});

    void update();
    void draw();

    Vector2 getPos() { return position; }
    int getRadius() { return radius; }

    void grab(Vector2 t_anchorPos);
    void held(Vector2 t_anchorPos, float t_dist);
    void released(Vector2 t_releaseDir);

private:
    int radius = 20;
    Texture2D texture;
    Rectangle sourceRect = { 0, 0, 0.1, 0.1 };

    bool grabbed = false;
    float angle = 0.0f;

    Vector2 position;

    // Physics
    float mass = 10;
    float speed = 150.0f;
    const int MAX_SPEED = 260;
    Vector2 velocity;
    float anchorDist = -1.0f;

    float radiansToDegrees(float t_radians) { return t_radians * (180/3.14f); }
    float degreesToRadians(float t_degrees) { return t_degrees * (3.14f / 180); }
};