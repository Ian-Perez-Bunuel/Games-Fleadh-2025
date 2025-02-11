#include "../include/PlanetCard.h"


PlanetCard::PlanetCard(Texture2D t_planet, std::string t_description)
    : planet(t_planet), description(t_description)
{
}


void PlanetCard::draw()
{
    DrawTextureEx(base, position, 0, 1, WHITE);
    DrawTextureEx(base, {position.x, position.y + 100}, 0, 1, WHITE);
    DrawTextEx(font, description.c_str(), position, 20, 1, WHITE);
}