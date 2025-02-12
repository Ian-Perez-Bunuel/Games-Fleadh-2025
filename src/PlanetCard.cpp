#include "../include/PlanetCard.h"


PlanetCard::PlanetCard(Vector2 t_pos, Texture2D t_planet, std::string t_description)
    : position(t_pos), planet(t_planet), description(t_description)
{
    font = LoadFont("resources/dogicapixelbold.ttf");
    base = LoadTexture("resources/Art/hologram.png");
}


void PlanetCard::draw()
{
    DrawTextureEx(base, position, 0, 1.5f, WHITE);
    DrawTextureEx(planet, {position.x + 125, position.y + 75}, 0, 0.1f, WHITE);
    DrawTextEx(font, description.c_str(), {position.x + 25, position.y + 325}, 15, 2, WHITE);
}