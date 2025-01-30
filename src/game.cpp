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
        objects[i]->update(controller.getRightStickDir());
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
    //if (IsGamepadAvailable(0))
    //{
        controllerInput();
    //}
    //else
    //{
        mouseInput();
    //}
}

void Game::mouseInput()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (!player.getGrapple().isActive())
        {
            for (int i = 0; i < currentObjectAmount; i++)
            {
                if (CheckCollisionPointCircle(GetMousePosition(), objects[i]->getPos(), objects[i]->getRadius()))
                {
                    player.shootGrapple(objects[i]);
                }
            }
        }
        else
        {
            player.releaseGrapple();
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        objects.push_back(std::make_shared<Object>(GetMousePosition()));
        currentObjectAmount++;
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    // Player grapple
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        for (int i = 0; i < currentObjectAmount; i++)
        {
            if (CheckCollisionCircles(controller.getCursorPos(), controller.getCursorRadius(), objects[i]->getPos(), objects[i]->getRadius()))
            {
                player.shootGrapple(objects[i]);
            }
        }
    }
    else
    {
        if (player.getGrapple().isActive())
        {
            player.releaseGrapple();
        }
    }

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
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