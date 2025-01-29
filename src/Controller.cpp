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
            getButtonInput();
            getStickInput();   
            getTriggerInput();
        }
    }
}

void Controller::getButtonInput()
{
    buttons.reset(); // Reset all buttons to false
        // Check if button if an input is given
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT))
    {
        printf("Start Pressed");
        buttons.start = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT))
    {
        printf("Select Pressed");
        buttons.select = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
    {
        printf("X Pressed");
        buttons.X = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) 
    {
        printf("A Pressed");
        buttons.A = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
    {
        printf("B Pressed");
        buttons.B = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP))
    {
        printf("Y Pressed");
        buttons.Y = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
    {
        printf("LEFT_TRIGGER");
        buttons.leftBumper = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {
        printf("RIGHT_TRIGGER");
        buttons.rightBumper = true;
    }
}
void Controller::getTriggerInput()
{
    // Check if bumpers are pressed
    leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
    rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
}

void Controller::getStickInput()
{
    
}