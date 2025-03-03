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
    void initAchievements();

    bool update(Vector3 t_playerPos3D, Player& t_player);
    void drawMainPlanet();
    void drawOtherPlanets();

    Planet& getMainPlanet() { return planets[currentPlanet]; }

    void nextPlanet();
    static void increaseTimesHit() { timesHit++; }
    static void increasePowerfulHits() { powerHits++; }

    void reset();
private:

    std::vector<Planet> planets;

    Model model;

    static int timesHit;
    static int powerHits;
    int coresCollected = 0;

    static const int PLANET_AMOUNT = 6;
    int currentPlanet = 0;
};