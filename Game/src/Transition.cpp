#include "../include/Transition.h"
#include "../include/Globals.h"
#include "rlgl.h"

bool Transition::active = false;
Vector2 Transition::start[Transition::LINES_AMOUNT];
Vector2  Transition::end[Transition::LINES_AMOUNT];
float Transition::lineDir[Transition::LINES_AMOUNT];

float Transition::targetSpeed = 100.0f;
float Transition::endElapsedTime = 0.0f;  
float Transition::startElapsedTime = 0.0f;

Sound Transition::sound;

Color Transition::color = {143, 45, 250, 255};

void Transition::initSound()
{
    sound = LoadSound("resources/Sound/transition.wav");
}

void Transition::begin()
{
    endElapsedTime = 0.0f;
    startElapsedTime = 0.0f;
    // Set all values
    for (int i = 0; i < LINES_AMOUNT; i++)
    {
        start[i] = {(float)(rand() % SCREEN_WIDTH), (float)(rand() % SCREEN_HEIGHT)};
        end[i] = start[i]; // Begin in the same position

        Vector2 center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
        // Get the angle towards the center
        lineDir[i] = atan2f(center.y - start[i].y, center.x - start[i].x) + PI;
    }

    SetSoundVolume(sound, 0.4f);
    PlaySound(sound);

    active = true;
}

void Transition::update()
{
    if (active)
    {
        Vector2 direction;
        float endsDuration = 0.8f;      // Total time to reach full speed
        float startsDuration = 0.2f;

        if(endElapsedTime < endsDuration)
        {
            endElapsedTime += GetFrameTime(); 
        }
        else
        {
            startElapsedTime += GetFrameTime();
        }
        

        for (int i = 0; i < LINES_AMOUNT; i++)
        {
            float endT = endElapsedTime / endsDuration;
            float endEasedT = endT * endT * endT * endT;
            float endSpeed = targetSpeed * endEasedT; 

            direction.x = endSpeed * cos(lineDir[i]);
            direction.y = endSpeed * sin(lineDir[i]);

            end[i].x += direction.x;
            end[i].y += direction.y;

            if (startElapsedTime < startsDuration)
            {
                float startT = startElapsedTime / startsDuration;
                float startEasedT = startT * startT * startT * startT;
                float startSpeed = targetSpeed * startEasedT; 

                direction.x = startSpeed * cos(lineDir[i]);
                direction.y = startSpeed * sin(lineDir[i]);

                start[i].x += direction.x;
                start[i].y += direction.y;
            }
            else
            {
                active = false;
            }
        }
    }
}

void Transition::draw()
{
    if (active)
    {
        rlSetLineWidth(2.0f);
        for (int i = 0; i < LINES_AMOUNT; i++)
        {
            DrawLineV(start[i], end[i], color);
        }
    }
}