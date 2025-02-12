#include "raylib.h"
#include "stdio.h"
#include <string>


class PlanetCard
{
public:
    PlanetCard(Vector2 t_pos, Texture2D t_planet, std::string t_description);

    void draw();

private:
    Vector2 position;

    Texture2D base;
    Texture2D planet;

    Font font;
    std::string description;
};