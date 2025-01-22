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

    for (Projectile& projectile : projectiles)
    {
        // Check if theyre colliding
        for (Planet planet : planets)
        {
            if (CircleCollisions(planet.getGravityRadius(), projectile.getRadius(), planet.getPos(), projectile.getPos()))
            {
                projectile.moveToPlanet(planet.getGravity(), planet.getPos());
            }
        }

        // Move projectiles
        projectile.move();
    }
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);

    for (Planet e : planets)
    {
        e.draw();
    }
    for (Projectile e : projectiles)
    {
        e.draw();
    }
}

void Game::input()
{
    // Spawn a planet
    if (IsMouseButtonReleased(0))
    {
        planets.push_back(Planet(GetMousePosition()));
    }
    // Spawn a projectile
    if (IsMouseButtonReleased(1))
    {
        projectiles.push_back(Projectile(GetMousePosition()));
    }
}



bool Game::CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2)
{
    // Calculate the squared distance between the two positions
    double distanceSquared = std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2);
    
    // Calculate the squared sum of the radii
    int radiusSum = t_r1 + t_r2;
    double radiusSumSquared = std::pow(radiusSum, 2);
    
    // Check if the distance squared is less than or equal to the squared radius sum
    return distanceSquared <= radiusSumSquared;
}