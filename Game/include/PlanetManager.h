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

    void update(Vector3 t_playerPos3D, Vector2 t_playerPos2D);
    void drawMainPlanet();
    void drawOtherPlanets();

    Planet& getMainPlanet() { return planets[currentPlanet]; }

    void nextPlanet();
private:

    std::vector<Planet> planets;

    static const int PLANET_AMOUNT = 5;
    int currentPlanet = 0;
};