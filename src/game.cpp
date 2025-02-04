#include "../include/game.h"
#include "../include/Globals.h"


Game::Game()
{
    printf("Game Initialized!\n");
}

Game::~Game()
{
    // Use to free resources
}

void Game::update() 
{
    input();

    player.update(controller.getLeftStickDir());

    for (int i = 0; i < currentObjectAmount; i++)
    {
        objects[i]->update();
    }
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);


    player.draw();
    for (int i = 0; i < currentObjectAmount; i++)
    {
        objects[i]->draw();
    }

    // Controller cursor
    if (IsGamepadAvailable(0))
    {
        controller.drawCursor();
    }
}

void Game::input()
{
    if (IsGamepadAvailable(0))
    {
        controllerInput();
    }
    else
    {
        mouseInput();
    }
}

void Game::mouseInput()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        player.releaseGrapple(GetMousePosition());
    }

    // Used to grab objects
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        for (int i = 0; i < currentObjectAmount; i++)
        {
            if (CheckCollisionPointCircle(GetMousePosition(), objects[i]->getPos(), objects[i]->getRadius()))
            {
                player.shootGrapple(objects[i]);
            }
        }
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        objects.push_back(std::make_shared<Object>(GetMousePosition()));
        currentObjectAmount++;
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple(GetMousePosition());
    }

    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        for (int i = 0; i < currentObjectAmount; i++)
        {
            if (CheckCollisionPointCircle(controller.getCursorPos(), objects[i]->getPos(), objects[i]->getRadius()))
            {
                player.shootGrapple(objects[i]);
            }
        }
    }

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        objects.push_back(std::make_shared<Object>(controller.getCursorPos()));
        currentObjectAmount++;
    }
}

bool Game::CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2)
{
    // Calculate the squared distance between the two positions
    double distanceSquared = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2);
    
    // Calculate the squared sum of the radii
    int radiusSum = t_r1 + t_r2;
    double radiusSumSquared = pow(radiusSum, 2);
    
    // Check if the distance squared is less than or equal to the squared radius sum
    return distanceSquared <= radiusSumSquared;
}