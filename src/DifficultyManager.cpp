#include "../include/DifficultyManager.h"

DifficultySettings DifficultyManager::currentDifficulty;
DifficultySettings DifficultyManager::originalDifficulty;

DifficultySettings DifficultyManager::easy;
DifficultySettings DifficultyManager::medium;
DifficultySettings DifficultyManager::hard;
DifficultySettings DifficultyManager::extreme;

void DifficultyManager::initBaseDifficulties()
{
    // Easy
    easy.batchSpawnAmount = 2;
    easy.objectMinimum = 20;
    easy.ordinanceSpacing = 5.0f;
    easy.pickupChance = 3;
    easy.ordinanceSpeed = 0.1f;

    // Medium
    medium.batchSpawnAmount = 3;
    medium.objectMinimum = 30;
    medium.ordinanceSpacing = 3.5f;
    medium.pickupChance = 5;
    medium.ordinanceSpeed = 0.15f;

    // Hard
    hard.batchSpawnAmount = 5;
    hard.objectMinimum = 50;
    hard.ordinanceSpacing = 1.5f;
    hard.pickupChance = 7;
    hard.ordinanceSpeed = 0.22f;

    // Extreme
    extreme.batchSpawnAmount = 5;
    extreme.objectMinimum = 150;
    extreme.ordinanceSpacing = 0.8f;
    extreme.pickupChance = 10;
    extreme.ordinanceSpeed = 0.3f;
}

void DifficultyManager::setDifficulty(DifficultySettings t_difficulty)
{
    originalDifficulty = t_difficulty;
    currentDifficulty = t_difficulty;
}

void DifficultyManager::resetDifficulty()
{
    currentDifficulty = originalDifficulty;
}

void DifficultyManager::increaseDifficulty()
{
    if (currentDifficulty.batchSpawnAmount < 5)
    {
        currentDifficulty.batchSpawnAmount++;
    }

    currentDifficulty.objectMinimum += 5;

    currentDifficulty.ordinanceSpacing *= 0.6f;
    currentDifficulty.ordinanceSpeed *= 1.2f;

    // I dont think scaling pickup chance through the game is a good idea
}

DifficultySettings DifficultyManager::getDifficulty(int t_level)
{
    switch (t_level)
    {
    case 0:
        return easy;
        break;

    case 1:
        return medium;
        break;

    case 2:
        return hard;
        break;

    case 3:
        return extreme;
        break;
    }

    return easy;
}