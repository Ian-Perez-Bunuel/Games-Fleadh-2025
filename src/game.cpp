#include "raylib.h"
#include "stdio.h"
#include "../include/Globals.h"
#include "../include/game.h"

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

}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);
}

