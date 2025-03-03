#include "../include/Achievement.h"
#include "../include/Globals.h"
#include "../include/reasings.h"
#include "rlgl.h"

#include <sstream>


Achievement::Achievement(Texture2D& t_texture, std::string t_title, AchievementType t_type)
    : texture(t_texture), title(t_title), type(t_type)
{
    font = LoadFontEx("resources/dogicapixelbold.ttf", FONT_SIZE, 0, 0);

    position = {-100, 50};

    switch (type)
    {
    case AchievementType::NONE:
        titleColor = WHITE;
        break;

    case AchievementType::OBJECT:
        titleColor = BLUE;
        break;

    case AchievementType::PLANET:
        titleColor = RED;
        break;

    case AchievementType::PLAYER:
        titleColor = {255, 228, 44, 255};
        break;
    }
}

void Achievement::addGoal(int* t_valueToKeepTrackOf, int t_goal)
{
    printf("\n\n%s - Has been set\n\n", title.c_str());
    value = t_valueToKeepTrackOf;
    goal = t_goal;
}

bool Achievement::checkIfCompleted()
{
    if (value == nullptr)
    {
        printf("\n\n\"%s\" s value and goal hasn't been set.\n\n", title.c_str());
        return false;
    }

    if (*value >= goal && !completed)
    {
        completed = true;
        show = true;

        printf("\n%s - Has been completed\n", title.c_str());

        return true;
    }

    return false;
}

bool Achievement::showClock()
{
    if (showTimer < SHOW_DURATION)
    {
        showTimer += GetFrameTime();
        return true;
    }
    else
    {
        show = false;
        return false;
    }
}


void Achievement::draw(float t_yOffset)
{
    if (completed && showClock())
    {
        if (animationTimer < 1.0f)
        {
            animationTimer += 0.1f;
        }

        position.x = lerp(-WIDTH, X_POSITION, animationTimer);
        position.y = (t_yOffset * HEIGHT) + 55;

        float difference = END_SCALE - START_SCALE;
        if (scaleTimer < 1.5f)
        {
            scaleTimer += GetFrameTime();
            scale = EaseElasticOut(scaleTimer, START_SCALE, difference, 1.5f);
        }
        else
        {
            scale = END_SCALE;
        }

        DrawTextureEx(texture, position, 0, scale, WHITE);

        Vector2 textSize = MeasureTextEx(font, splitSentence(title).c_str(), FONT_SIZE, 5.0f);
        Vector2 titlePos = {position.x + ((texture.width * scale) - textSize.x) / 2, position.y + 25};
        DrawTextEx(font, splitSentence(title).c_str(), titlePos, FONT_SIZE, 5.0f, titleColor);
    }
}


std::string Achievement::splitSentence(std::string t_original)
{
    // Default parameters for text wrapping.
    const int maxWidth = 50;
    const float spacing = 5.0f;
    // Get the default font (requires an initialized window).
    Font font = GetFontDefault();

    std::istringstream words(t_original);
    std::string word;
    std::string line;
    std::string result;
    
    while (words >> word)
    {
        // Create a candidate line by adding the next word.
        std::string testLine = line.empty() ? word : line + " " + word;
        Vector2 size = MeasureTextEx(font, testLine.c_str(), (float)FONT_SIZE, spacing);
        
        // If the test line exceeds maxWidth, append the current line and start a new one.
        if (size.x > maxWidth)
        {
            if (!line.empty())
            {
                result += line + "\n\n";
                line = word;
            }
            else
            {
                // If a single word exceeds maxWidth, add it as its own line.
                result += word + "\n\n";
                line.clear();
            }
        }
        else
        {
            if (!line.empty())
                line += " ";
            line += word;
        }
    }
    
    // Append any remaining text.
    if (!line.empty())
    {
        result += line;
    }
    
    return result;
}