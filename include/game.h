#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdio.h"
#include <vector>

#include "../include/Player.h"
#include "../include/Controller.h"


class Game
{
public:
    Game();
    ~Game();

    void initialize();
    void update();
    void draw();

private:
    void input();
    void mouseInput();
    void controllerInput();
    Controller controller;
    
    Player player;

    
    bool CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2);

    static const int MAX_OBJECTS;
    int currentObjectAmount;
    std::vector<std::shared_ptr<Object>> objects;

    // Shaders
    float glowRadius = 14.0f;
    float glowIntensity = 5.0f;
    Shader blurHorizontal;
    Shader blurVertical;
    Shader combineShader;
    // Render textures for multi pass shaders
    RenderTexture2D targetScene;
    RenderTexture2D targetBlur1;
    RenderTexture2D targetBlur2;
};

#endif // GAME_H