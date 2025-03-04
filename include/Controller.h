#pragma once

#include "raylib.h"

struct Buttons
{
    bool A = false;
    bool B = false;
    bool X = false;
    bool Y = false;

    bool leftBumper = false;
    bool rightBumper = false;

    bool start = false;
    bool select = false;

    void reset()
    {
        A = false;
        B = false;
        X = false;
        Y = false;

        leftBumper = false;
        rightBumper = false;

        start = false;
        select = false;
    }
};

class Controller
{
public:
    void init();
    void getAllInputs();

    // Get info on triggers
    float getLeftDepth() { return leftTrigger; }
    float getRightDepth() { return rightTrigger; }

    // Get info on sticks
    Vector2 getLeftStickDir() { return leftStick; }
    Vector2 getRightStickDir() { return rightStick; }

    Buttons buttons;

    // Cursor
    Vector2 getCursorPos() { return cursorPos; }
    int getCursorRadius() { return CURSOR_RADIUS; }
    void drawCursor();
    bool visable = false;

private:
    void getButtonInput();
    void getTriggerInput();
    void getStickInput();
    
    int gamepad = 0; // which gamepad being used

    // Trigger depth
    float leftTrigger = 0;
    float rightTrigger = 0;

    Model reticle;

    // Stick Direction
    Vector2 leftStick = { 0.0f, 0.0f };
    Vector2 rightStick = { 0.0f, 0.0f };

    // Cursor
    void updateCursor();
    Vector2 cursorPos;
    const int CURSOR_RADIUS = 40;
    
    // Set axis deadzones
    const float LEFT_STICK_DEADZONE_X = 0.2f;
    const float LEFT_STICK_DEADZONE_Y = 0.2f;
    const float RIGHT_STICK_DEADZONE_X = 0.2f;
    const float RIGHT_STICK_DEADZONE_Y = 0.2f;
    const float LEFT_TRIGGER_DEADZONE = -0.9f;
    const float RIGHT_TRIGGER_DEADZONE = -0.9f;

    float normalizeSigned(float x, float t_min, float t_max) 
    {
        return 2 * (x - t_min) / (t_max - t_min) - 1;
    }
    Vector3 convertToMiddleCoords(Vector2 t_originalCoords);
};