#include "../include/PlanetManager.h"
#include "../include/Globals.h"

#include "../include/DifficultyManager.h"
#include "../include/Transition.h"
#include "../include/AchievementManager.h"
#include "../include/SceneCamera.h"

int PlanetManager::timesHit = 0;
int PlanetManager::powerHits = 0;

void PlanetManager::init()
{    
    initAchievements();
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

        float randX = static_cast<float>((rand() % (2 * (int)SCREEN_BOUNDS_X)) - SCREEN_BOUNDS_X);
        float randY = static_cast<float>((rand() % (2 * (int)SCREEN_BOUNDS_Y)) - SCREEN_BOUNDS_Y);

        // Health goes from 100 up in 50

        planets[i].init({randX, randY, MAIN_PLANET_Z - (i * PLANET_SPACING)}, 100 + i * 50, randomColor);
        planets[i].turnOnProjectiles();
    }
}

void PlanetManager::initAchievements()
{
    AchievementManager::addGoalToAchievement("First Blood", &timesHit, 1);
    AchievementManager::addGoalToAchievement("Nice Aim!!", &timesHit, 10);
    AchievementManager::addGoalToAchievement("Resolve Is Key", &timesHit, 25);
    AchievementManager::addGoalToAchievement("COMBO!", &timesHit, 50);
    AchievementManager::addGoalToAchievement("ORA! ORA! ORA!", &timesHit, 100);

    AchievementManager::addGoalToAchievement("Powerful Hit!", &powerHits, 1);

    AchievementManager::addGoalToAchievement("Core Collector", &coresCollected, 1);
    AchievementManager::addGoalToAchievement("Almost there!", &coresCollected, 3);
    AchievementManager::addGoalToAchievement("3D Achieved!!!", &coresCollected, 5);
    AchievementManager::addGoalToAchievement("Congratulations!!!", &coresCollected, 6);
}

bool PlanetManager::update(Vector3 t_playerPos3D, Player& t_player)
{
    // Ensure currentPlanet is valid before updating.
    if (!endScene)
    {
        if (currentPlanet >= planets.size()) 
        {
            return false;
        }

        planets[currentPlanet].update(t_playerPos3D, t_player);

        if (planets[currentPlanet].isCoreConsumed() && planets[currentPlanet].checkIfParticlesActive())
        {
            coresCollected++;
            Transition::begin();
            t_player.dropEverything();
            nextPlanet();
            return true;
        }
    }


    return false;
}

void PlanetManager::drawMainPlanet()
{
    if (currentPlanet < planets.size() && !endScene)
    {
        planets[currentPlanet].draw();
    }
}

void PlanetManager::drawOtherPlanets()
{
    for (int i = currentPlanet + 1; i < planets.size(); i++)
    {
        if (!endScene)
        {
            planets[i].draw();
        }
    }
}

void PlanetManager::nextPlanet()
{
    // Only increment if we're not at the last planet.
    if (currentPlanet < planets.size() - 1 && !endScene)
    {
        currentPlanet++;
        DifficultyManager::increaseDifficulty();
        
        // Adjust positions of all planets.
        for (Planet& planet : planets)
        {
            Vector3 pos = planet.getPos();
            planet.setPos({ pos.x, pos.y, pos.z + PLANET_SPACING });
        }
    }
    else
    {
        endScene = true;
    }
}

void PlanetManager::reset()
{
    endScene = false;
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

        float randX = static_cast<float>((rand() % (2 * (int)SCREEN_BOUNDS_X)) - SCREEN_BOUNDS_X);
        float randY = static_cast<float>((rand() % (2 * (int)SCREEN_BOUNDS_Y)) - SCREEN_BOUNDS_Y);

        planets[i].init({randX, randY, MAIN_PLANET_Z - (i * PLANET_SPACING)}, 100 + i * 50, randomColor);
    }
}