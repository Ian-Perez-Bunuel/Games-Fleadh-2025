#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <memory>
#include <vector>

class Particle
{
public:
    Particle(Vector2 t_pos = {0, 0}, float t_size = 1.0f, float t_speed = 1.0f, float t_dirAngle = 0.0f, Color t_color = WHITE);
    ~Particle() { active = false; }

    void move();
    void draw();

    bool isActive() { return active; }

private:
    Vector2 position;
    Vector2 velocity;

    bool active = false;

    float size;
    const float MIN_SIZE = 0.2f;
    float speed;
    const int MIN_SPEED = 2;
    float dirAngle;

    Color color;
};