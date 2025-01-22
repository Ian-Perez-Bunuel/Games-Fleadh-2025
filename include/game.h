#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdio.h"
#include <vector>

#include "../include/Player.h"
#include "../include/Object.h"

class Game
{
public:
    Game();
    ~Game();

    void update();
    void draw();

private:

    Player player;

    std::vector<Object> objects;
    int amount = 0;

};

#endif // GAME_H