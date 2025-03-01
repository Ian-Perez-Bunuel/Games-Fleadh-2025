#pragma once

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include <string>

#include "../include/Achievement.h"

class AchievementManager
{
public:
    void init();
    static void addGoalToAchievement(std::string t_achievementTitle, int* t_valueToKeepTrackOf, int t_goal);

    void checkForChanges();
    void draw();

private:
    static std::vector<Achievement> achievements;
};