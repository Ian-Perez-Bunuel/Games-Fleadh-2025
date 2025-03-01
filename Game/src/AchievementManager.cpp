#include "../include/AchievementManager.h"

std::vector<Achievement> AchievementManager::achievements;

void AchievementManager::init()
{
    // Type::NONE
    achievements.push_back(Achievement("Welcome To The World!", "You litterly did nothing", AchievementType::NONE));

    // Type::PLAYER
    achievements.push_back(Achievement("GRAB, GRAB, GRAB!!!", "Grabbed your first Object", AchievementType::PLAYER));

    // Type::OBJECT


    // Type::PLANET

}

void AchievementManager::checkForChanges()
{
    for (Achievement& a : achievements)
    {
        a.checkIfCompleted();
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