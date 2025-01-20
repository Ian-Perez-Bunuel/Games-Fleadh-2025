#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdio.h"

#include "../include/Player.h"

class Game
{
public:
    Game();
    ~Game();

    void update();
    void draw();

private:

    Player player;

};

#endif // GAME_H