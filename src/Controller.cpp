#include "../include/Controller.h"
#include "stdio.h"

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS     "playstation"


void Controller::getAllInputs()
{
    if (IsGamepadAvailable(gamepad))
    {
        if (TextFindIndex(TextToLower(GetGamepadName(gamepad)), XBOX_ALIAS_1) > -1 || TextFindIndex(TextToLower(GetGamepadName(gamepad)), XBOX_ALIAS_2) > -1)
        {
            updateCursor();

            getButtonInput();
            getStickInput();   
            getTriggerInput();
        }
    }
}

void Controller::drawCursor()
{
    DrawCircleLinesV(cursorPos, CURSOR_RADIUS, BLUE); 
}

void Controller::updateCursor()
{
    cursorPos.x += rightStick.x * 10;
    cursorPos.y += rightStick.y * 10;
}

void Controller::getButtonInput()
{
    buttons.reset(); // Reset all buttons to false
        // Check if button if an input is given
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT))
    {
        buttons.start = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT))
    {
        buttons.select = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
    {
        buttons.X = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) 
    {
        buttons.A = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
    {
        buttons.B = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP))
    {
        buttons.Y = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
    {
        buttons.leftBumper = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {
        buttons.rightBumper = true;
    }
}

void Controller::getTriggerInput()
{
    // Check if bumpers are pressed
    leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);   // Gamepad button 10
    rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER); // Gamepad button 12

    // Calculate deadzones
    if (leftTrigger < LEFT_TRIGGER_DEADZONE) 
    {
        leftTrigger = -1.0f;
    }
    if (rightTrigger < RIGHT_TRIGGER_DEADZONE) 
    {
        rightTrigger = -1.0f;
    }
}

void Controller::getStickInput()
{
    // Left stick
    // Axis 0 = X   // Left: -1.0, Right: 1.0
    // Axis 1 = Y   // Up: -1.0, Down: 1.0
    leftStick.x = GetGamepadAxisMovement(gamepad, 0);
    leftStick.y = GetGamepadAxisMovement(gamepad, 1);

    // Right stick
    // Axis 2 = X   // Left: -1.0, Right: 1.0
    // Axis 3 = Y   // Up: -1.0, Down: 1.0
    rightStick.x = GetGamepadAxisMovement(gamepad, 2);
    rightStick.y = GetGamepadAxisMovement(gamepad, 3);

    // Force deadzones
    if (leftStick.x > -LEFT_STICK_DEADZONE_X && leftStick.x < LEFT_STICK_DEADZONE_X) leftStick.x = 0.0f;
    if (leftStick.y > -LEFT_STICK_DEADZONE_Y && leftStick.y < LEFT_STICK_DEADZONE_Y) leftStick.y = 0.0f;
    if (rightStick.x > -RIGHT_STICK_DEADZONE_X && rightStick.x < RIGHT_STICK_DEADZONE_X) rightStick.x = 0.0f;
    if (rightStick.y > -RIGHT_STICK_DEADZONE_Y && rightStick.y < RIGHT_STICK_DEADZONE_Y) rightStick.y = 0.0f;
}