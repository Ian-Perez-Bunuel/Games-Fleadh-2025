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
    void initializeShaders();

    void input();
    void mouseInput();
    void controllerInput();
    Controller controller;
    
    Player player;

    
    bool CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2);

    static const int MAX_OBJECTS;
    int currentObjectAmount;
    std::vector<std::shared_ptr<Object>> objects;

    Texture2D background;
    Texture2D enemy;

    // Shaders
    float glowRadius = 10.0f;
    float glowIntensity = 0.5f;
    Shader blurHorizontal;
    Shader blurVertical;
    Shader combineShader;
    Shader crtShader;
    // Render textures for multi pass shaders
    RenderTexture2D targetScene;
    RenderTexture2D targetBlur1;
    RenderTexture2D targetBlur2;
};

#endif // GAME_H