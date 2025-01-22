#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include <math.h>

#include "../include/Planet.h"
#include "../include/Projectile.h"

class Game
{
public:
    Game();
    ~Game();

    void update();
    void draw();

private:
    void input();
    

    bool CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2);

    std::vector<Planet> planets;
    std::vector<Projectile> projectiles;
};

#endif // GAME_H