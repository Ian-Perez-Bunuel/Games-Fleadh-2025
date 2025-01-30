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

    bool getLeftTrigger() { return leftTriggerPressed; }
    bool getRightTrigger() { return rightTriggerPressed; }

private:
    void getButtonInput();
    void getTriggerInput();
    void getStickInput();
    
    int gamepad = 0; // which gamepad being used

    // Trigger depth
    float leftTrigger = 0;
    float rightTrigger = 0;
    bool leftTriggerPressed = false;
    bool rightTriggerPressed = false;

    // Stick Direction
    Vector2 leftStick = { 0.0f, 0.0f };
    Vector2 rightStick = { 0.0f, 0.0f };

    // Cursor
    void updateCursor();
    Vector2 cursorPos  = { 400.0f, 400.0f};
    const int CURSOR_RADIUS = 40;
    
    // Set axis deadzones
    const float LEFT_STICK_DEADZONE_X = 0.2f;
    const float LEFT_STICK_DEADZONE_Y = 0.2f;
    const float RIGHT_STICK_DEADZONE_X = 0.2f;
    const float RIGHT_STICK_DEADZONE_Y = 0.2f;
    const float LEFT_TRIGGER_DEADZONE = -0.9f;
    const float RIGHT_TRIGGER_DEADZONE = -0.9f;
};