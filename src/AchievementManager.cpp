#include "../include/AchievementManager.h"

std::vector<Achievement> AchievementManager::achievements;
Sound AchievementManager::achievementComplete;

void AchievementManager::init()
{
    // Texture
    box = LoadTexture("resources/Art/2D/achievementBox.png");
    // Sound
    achievementComplete = LoadSound("resources/Sound/achievement.wav");
    SetSoundVolume(achievementComplete, 0.2);


    // Type::NONE
    achievements.push_back(Achievement(box, "Hello World!", AchievementType::NONE));

    // Type::PLAYER
    achievements.push_back(Achievement(box, "Baby's first toy", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "GRAB, GRAB, GRAB!", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "ALL FULL!!!", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "Asteroid Lover", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "Rock Collector", AchievementType::PLAYER));

    achievements.push_back(Achievement(box, "Engines On!", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "Marathon Runner", AchievementType::PLAYER));

    // Type::OBJECT
    achievements.push_back(Achievement(box, "Speed Star!", AchievementType::OBJECT));
    achievements.push_back(Achievement(box, "Particle Accelerator", AchievementType::OBJECT));

    achievements.push_back(Achievement(box, "Powerful Hit!", AchievementType::OBJECT));

    // Type::PLANET
    achievements.push_back(Achievement(box, "First Blood", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "Nice Aim!!", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "Resolve Is Key", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "COMBO!", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "ORA! ORA! ORA!", AchievementType::PLANET));

    achievements.push_back(Achievement(box, "Core Collector", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "Almost there!", AchievementType::PLANET));
    achievements.push_back(Achievement(box, "3D Achieved!!!", AchievementType::PLANET));
}

void AchievementManager::checkForChanges()
{
    for (Achievement& a : achievements)
    {
        if (a.checkIfCompleted())
        {
            SetSoundPitch(achievementComplete, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
            PlaySound(achievementComplete);
        }
    }
}

void AchievementManager::draw()
{
    achievementAmount = -1;
    for (Achievement& a : achievements)
    {
        if (a.isOnShow())
        {
            achievementAmount++;
            a.draw(achievementAmount);
        }

    }
}

void AchievementManager::addGoalToAchievement(std::string t_achievementTitle, int *t_valueToKeepTrackOf, int t_goal)
{
    bool found = false;
    for (Achievement& a : achievements)
    {
        if (a.getTitle() == t_achievementTitle)
        {
            found = true;
            a.addGoal(t_valueToKeepTrackOf, t_goal);
        }
    }

    if (!found)
    {
        printf("\n\nCouldnt find: %s\n\n", t_achievementTitle.c_str());
    }

    if (t_valueToKeepTrackOf == nullptr)
    {
        printf("\n\nVALUE IS NULLPTR\n\n");
    }
}