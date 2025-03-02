#include "../include/Achievement.h"
#include "../include/Globals.h"
#include "rlgl.h"

#include <sstream>


Achievement::Achievement(Texture2D& t_texture, std::string t_title, std::string t_description, AchievementType t_type)
    : texture(t_texture), title(t_title), description(t_description), type(t_type)
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

        // DrawRectangleLines(position.x, position.y, WIDTH, HEIGHT, BLUE);
        DrawTextureEx(texture, position, 0, 0.6f, WHITE);

        Vector2 titlePos = {position.x + 15, position.y + 25};
        DrawTextEx(font, splitSentence(title).c_str(), titlePos, FONT_SIZE, 0.5f, titleColor);

        Vector2 descriptionPos = {position.x + 15, position.y + 80};
        DrawTextEx(font, splitSentence(description).c_str(), descriptionPos, FONT_SIZE, 0.5f, WHITE);
    }
}


std::string Achievement::splitSentence(std::string t_original)
{
    std::istringstream words(t_original);
    std::string word, line, result;
    
    while (words >> word) {
        // Check if adding the next word exceeds the line length.
        if (!line.empty() && (line.size() + word.size() + 1 > CHARACTERS_PER_LINE)) {
            result += line + "\n";  // Append the current line with newline.
            line = word;            // Start a new line with the current word.
        } else {
            if (!line.empty())
                line += " "; // Add a space between words.
            line += word;
        }
    }
    // Append any remaining text.
    if (!line.empty())
        result += line;
    
    return result;
}