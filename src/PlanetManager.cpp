#include "../include/PlanetManager.h"
#include "../include/Globals.h"

#include "../include/DifficultyManager.h"
#include "../include/Transition.h"

void PlanetManager::init()
{
    planets.clear();

    for (int i = 0; i < PLANET_AMOUNT; i++)
    {
        planets.push_back(Planet());

        const int TOTAL = 326;
        Color randomColor;

        // Pick r anywhere from 0 to 255.
        int r = rand() % 256;

        // For a valid triple, we must have 0 <= g <= 255 and also b = TOTAL - r - g must be between 0 and 255.
        // This forces: TOTAL - r - g >= 0   and   TOTAL - r - g <= 255,
        // which is equivalent to: g <= TOTAL - r   and   g >= TOTAL - r - 255.

        // So, compute the allowed range for g:
        int g_min = (TOTAL - r - 255 > 0) ? TOTAL - r - 255 : 0;
        int g_max = (TOTAL - r < 255) ? TOTAL - r : 255;

        // Now pick g from that range.
        int g = g_min + (rand() % (g_max - g_min + 1));

        // And b is determined by the constraint.
        int b = TOTAL - r - g;

        randomColor.r = r;
        randomColor.g = g;
        randomColor.b = b;
        randomColor.a = 255;

        printf("\n\n%f, %f, %f\n\n", randomColor.r, randomColor.g, randomColor.b);

        float randX = static_cast<float>((rand() % (2 * SCREEN_BOUNDS_X)) - SCREEN_BOUNDS_X);
        float randY = static_cast<float>((rand() % (2 * SCREEN_BOUNDS_Y)) - SCREEN_BOUNDS_Y);

        // Health goes from 100 up in 50

        planets[i].init({randX, randY, MAIN_PLANET_Z - (i * PLANET_SPACING)}, 100 + i * 50, randomColor);
        planets[i].turnOnProjectiles();
    }
}

void PlanetManager::update(Vector3 t_playerPos3D, Player& t_player)
{
    planets[currentPlanet].update(t_playerPos3D, t_player);

    if (planets[currentPlanet].isCoreConsumed() && planets[currentPlanet].checkIfParticlesActive())
    {
        Transition::begin();
        nextPlanet();
    }
}

void PlanetManager::drawMainPlanet()
{
    planets[currentPlanet].draw();
}

void PlanetManager::drawOtherPlanets()
{
    for (int i = currentPlanet + 1; i < PLANET_AMOUNT; i++)
    {
        planets[i].draw();
    }
}

void PlanetManager::nextPlanet()
{
    if (currentPlanet < planets.size())
    {
        currentPlanet++;
        DifficultyManager::increaseDifficulty();
        
        for (Planet& planet : planets)
        {
            planet.setPos({planet.getPos().x, planet.getPos().y, planet.getPos().z + PLANET_SPACING});
        }
    }

}

void PlanetManager::reset()
{
    currentPlanet = 0;

    const int TOTAL = 326;
    Color randomColor;
    
    for (int i = 0; i < PLANET_AMOUNT; i++)
    {
        // Pick r anywhere from 0 to 255.
        int r = rand() % 256;

        // For a valid triple, we must have 0 <= g <= 255 and also b = TOTAL - r - g must be between 0 and 255.
        // This forces: TOTAL - r - g >= 0   and   TOTAL - r - g <= 255,
        // which is equivalent to: g <= TOTAL - r   and   g >= TOTAL - r - 255.

        // So, compute the allowed range for g:
        int g_min = (TOTAL - r - 255 > 0) ? TOTAL - r - 255 : 0;
        int g_max = (TOTAL - r < 255) ? TOTAL - r : 255;

        // Now pick g from that range.
        int g = g_min + (rand() % (g_max - g_min + 1));

        // And b is determined by the constraint.
        int b = TOTAL - r - g;

        randomColor.r = r;
        randomColor.g = g;
        randomColor.b = b;
        randomColor.a = 255;

        float randX = static_cast<float>((rand() % (2 * SCREEN_BOUNDS_X)) - SCREEN_BOUNDS_X);
        float randY = static_cast<float>((rand() % (2 * SCREEN_BOUNDS_Y)) - SCREEN_BOUNDS_Y);

        planets[i].init({randX, randY, MAIN_PLANET_Z - (i * PLANET_SPACING)}, 100 + i * 50, randomColor);
        planets[i].setAlive();
    }
}