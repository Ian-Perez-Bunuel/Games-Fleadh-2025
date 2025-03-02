#pragma once

#include "raylib.h"
#include "stdio.h"

struct DifficultySettings
{
    // Objects
    int objectMinimum = 35;         // Minimum amount of objects on screen at once
    int batchSpawnAmount = 5;       // Anount of objects spawned per batch
    int pickupChance = 5;           // 1 / 5 chance

    // Planet
    float ordinanceSpacing = 1.5f;  // Time between planet attacks
    float ordinanceSpeed = 0.1f;    // Speed of the ordinance
};

class DifficultyManager
{
public:
    static void initBaseDifficulties();
    
    static void setDifficulty(DifficultySettings t_difficulty);
    static void increaseDifficulty();

    static int getObjectMin() { return currentDifficulty.objectMinimum; }
    static int getBatchSpawnAmount() { return currentDifficulty.batchSpawnAmount; }
    static int getPickupChance() { return currentDifficulty.pickupChance; }

    static float getOrdinanceSpacing() { return currentDifficulty.ordinanceSpacing; }
    static float getOrdinanceSpeed() { return currentDifficulty.ordinanceSpeed; }

    static DifficultySettings getDifficulty(int t_level);

private:
    
    static DifficultySettings currentDifficulty;

    static DifficultySettings easy;
    static DifficultySettings medium;
    static DifficultySettings hard;
    static DifficultySettings extreme;
};