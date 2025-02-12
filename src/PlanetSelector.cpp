#include "../include/PlanetSelector.h"
#include "../include/Globals.h"
#include <string>

void PlanetSelector::init()
{
    texture = LoadTexture("resources/Art/Planet.png");

    for (int i = 0; i < PLANET_AMOUNT; i++)
    {
        Vector2 position = {(i - 1.0f) * SPACING, (SCREEN_HEIGHT / 2.0f) - 300};
        planets.push_back(PlanetCard(position, texture, "This is planet: " + std::to_string(i + 1)));
        startingPos.push_back(position);
    }   

    int randomVar = SCREEN_WIDTH;
    randomVar++;
}

void PlanetSelector::draw()
{
    for (int i = 0; i < PLANET_AMOUNT; i++)
    {
        planets[i].draw();
    }
}

void PlanetSelector::transition()
{
    if (transitionTimer < TRANSITION_DURATION)
    {
        transitionTimer++;
        
        for (int i = 0; i < PLANET_AMOUNT; i++)
        {
            planets[i].moveTo({startingPos[i].x + SPACING, (SCREEN_HEIGHT / 2.0f) - 300}, speed);
        }
    }
    else
    {
        transitionTimer = 0;

        for (int i = 0; i < PLANET_AMOUNT; i++)
        {
            startingPos[i] = planets[i].getPos();
        }

        active = false;
    }
}