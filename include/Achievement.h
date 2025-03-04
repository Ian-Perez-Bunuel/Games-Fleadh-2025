#pragma once

#include "raylib.h"
#include "stdio.h"
#include <string>

enum class AchievementType
{
    NONE,
    OBJECT,
    PLANET,
    PLAYER
};

class Achievement
{
public:
    Achievement(Texture2D& t_texture, Font& t_font, std::string t_title, AchievementType t_type);
    void addGoal(int* t_valueToKeepTrackOf, int t_goal);

    void draw(float t_yOffset);
    bool isOnShow() { return show; }

    std::string getTitle() { return title; }
    bool hasBeenCompleted() { return completed; }

    bool checkIfCompleted();
    void lock(); 

private:
    bool completed = false;
    bool showClock();
    bool show = false;
    const float SHOW_DURATION = 5.0f;
    float showTimer = 0.0f;

    Vector2 position;

    static const int WIDTH = 275;
    static const int HEIGHT = 150;

    // Description
    Font& font;
    const int FONT_SIZE = 18;
    int extraLineSpacing = 20;
    const int CHARACTERS_PER_LINE = 18;

    std::string title;
    Color titleColor;
    
    AchievementType type;

    float scale = 1.0f;
    float scaleTimer = 0.0f;
    const float START_SCALE = 0.9f;
    const float END_SCALE = 0.6f;

    // Goal
    int* value = nullptr;
    int goal = 0;

    Texture2D& texture;

    const float X_POSITION = 100;
    float animationTimer = 0.0f;

    float lerp(float start, float end, float t) { return start + t * (end - start); }
    std::string splitSentence(std::string t_original);
};