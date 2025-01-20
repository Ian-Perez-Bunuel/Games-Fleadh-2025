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
    player.move();
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);

    player.draw();
}

