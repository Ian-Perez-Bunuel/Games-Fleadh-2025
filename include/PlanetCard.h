#include "raylib.h"
#include "stdio.h"
#include <string>


class PlanetCard
{
public:
    PlanetCard(Vector2 t_pos, Texture2D t_planet, std::string t_description);

    void draw();
    void moveTo(Vector2 t_targetPos, float t_speed);
    Vector2 getPos() { return position; }

private:
    Vector2 position;

    Texture2D base;
    Texture2D planet;

    Font font;
    std::string description;
};