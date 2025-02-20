#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <vector>

#include "../include/Planet.h"

class PlanetManager
{
public:
    void init();

    void update();
    void drawMainPlanet();
    void drawOtherPlanets();

    Planet& getMainPlanet() { return planets[0]; }

private:
    std::vector<Planet> planets;

    static const int PLANET_AMOUNT = 5;
    int currentPlanet = 0;

    const float MAIN_PLANET_Z = -9.0f;
    const int PLANET_SPACING = 100;
};