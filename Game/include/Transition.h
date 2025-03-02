#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <random>

class Transition
{
public:
    static void initSound();
    static void begin();

    static void update();
    static void draw();

    static bool isActive() { return active; }

private:
    static bool active;

    static const int LINES_AMOUNT = 5000;
    static Vector2 start[LINES_AMOUNT];
    static Vector2 end[LINES_AMOUNT];

    static float lineDir[LINES_AMOUNT];

    // Movement variables
    static float targetSpeed;
    static float endElapsedTime;
    static float startElapsedTime;

    static Sound sound;
    static Color color;
};