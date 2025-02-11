#include "raylib.h"
#include "stdio.h"
#include <string>

#include "../include/Globals.h"


class PlanetCard
{
public:
    PlanetCard(Texture2D t_planet, std::string t_description);

    void draw();

private:
    Vector2 position;

    Texture2D base;
    Texture2D planet;

    const Font font =  LoadFont("resources/dogicapixelbold.ttf");
    std::string description;
};