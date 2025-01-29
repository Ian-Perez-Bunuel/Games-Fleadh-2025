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
    void getAllInputs();

    Buttons buttons;

private:
    void getButtonInput();
    void getTriggerInput();
    void getStickInput();
    
    int gamepad = 0; // which gamepad being used

    float leftTrigger = 0;
    float rightTrigger = 0;
    
    // Set axis deadzones
    const float LEFT_STICK_DEADZONE_X = 0.1f;
    const float LEFT_STICK_DEADZONE_Y = 0.1f;
    const float RIGHT_LEFT_STICK_DEADZONE_X = 0.1f;
    const float RIGHT_LEFT_STICK_DEADZONE_Y = 0.1f;
    const float LEFT_TRIGGER_DEADZONE = -0.9f;
    const float RIGHT_TRIGGER_DEADZONE = -0.9f;
};