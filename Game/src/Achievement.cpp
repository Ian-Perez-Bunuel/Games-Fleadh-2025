#include "../include/Achievement.h"
#include "../include/Globals.h"
#include "rlgl.h"

#include <sstream>


Achievement::Achievement(std::string t_title, std::string t_description, AchievementType t_type)
    : title(t_title), description(t_description), type(t_type)
{
    font = LoadFont("resources/dogicapixelbold.ttf");

    position = {100, 50};

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

void Achievement::checkIfCompleted()
{
    if (value == nullptr)
    {
        printf("\n\n\"%s\" s value and goal hasn't been set.\n\n", title.c_str());
    }

    if (*value >= goal && !completed)
    {
        completed = true;
        show = true;

        printf("\n%s - Has been completed\n", title.c_str());
    }
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
        return false;
    }
}


void Achievement::draw()
{
    if (completed && showClock())
    {
        DrawRectangleLines(position.x, position.y, WIDTH, HEIGHT, BLUE);

        Vector2 titlePos = {position.x + 8, position.y + 15};
        DrawTextEx(font, splitSentence(title).c_str(), titlePos, FONT_SIZE, 0.5f, titleColor);

        Vector2 descriptionPos = {position.x + 8, position.y + 65};
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