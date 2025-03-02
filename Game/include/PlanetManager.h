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

    void update(Vector3 t_playerPos3D, Player& t_player);
    void drawMainPlanet();
    void drawOtherPlanets();

    Planet& getMainPlanet() { return planets[currentPlanet]; }

    void nextPlanet();
private:

    std::vector<Planet> planets;

    void genModel();
    Model model;
    Model planet1;
    Model planet2;
    Model planet3;
    Model planet4;

    static const int PLANET_AMOUNT = 6;
    int currentPlanet = 0;
};