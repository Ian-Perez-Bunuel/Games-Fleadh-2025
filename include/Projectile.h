#include "raylib.h"

class Projectile
{
public:
    Projectile(Vector2 t_pos);

    void draw();

    void move();
    void moveToPlanet(float t_gravity, Vector2 t_planetPos);

    int getRadius() { return radius; }
    Vector2 getPos() { return position; }

private:
    Vector2 position;
    int radius = 15;

    Vector2 velocity;

    const Vector2 target = {200, 200};

    Vector2 dirVector(Vector2 t_pos, Vector2 t_goToPos) { return {t_goToPos.x - t_pos.x, t_goToPos.y - t_pos.y}; }

};