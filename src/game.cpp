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
    player.update();

    for (int i = 0; i < amount; i++)
    {
        objects[i].update();
    }

    if (IsMouseButtonPressed(1))
    {
        objects.push_back(Object(GetMousePosition()));
        amount++;
    }
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);

    for (int i = 0; i < amount; i++)
    {
        objects[i].draw();
    }
}

