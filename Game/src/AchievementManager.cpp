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
    achievements.push_back(Achievement(box, "Welcome To The World!", "Begin playing the game", AchievementType::NONE));

    // Type::PLAYER
    achievements.push_back(Achievement(box, "Baby's first toy", "Grabbed your first Object", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "GRAB, GRAB, GRAB!", "Grab 5 objects", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "ALL FULL!!!", "Grab 8 objects at once", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "Asteroid Lover", "Grab 20 objects", AchievementType::PLAYER));

    achievements.push_back(Achievement(box, "Engines On!", "Move for the first time", AchievementType::PLAYER));
    achievements.push_back(Achievement(box, "Marathon Runner", "Move around a good bit", AchievementType::PLAYER));

    // Type::OBJECT
    achievements.push_back(Achievement(box, "Particle Accelerator", "Rotating object reached max speed", AchievementType::OBJECT));

    // Type::PLANET

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