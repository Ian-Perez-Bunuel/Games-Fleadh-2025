#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include <memory>

#include "../include/Controller.h"
#include "../include/SceneCamera.h"
#include "../include/PlanetSelector.h"
#include "../include/ObjectManager.h"


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

    void drawForeground();
    void drawBackground();

    void input();
    void mouseInput();
    void controllerInput();
    Controller controller;
    
    Player player;
    std::shared_ptr<Object> closestObjectToPlayer;

    // Reticle
    Texture2D reticle;

    
    bool CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2);

    ObjectManager objectManager;

    Texture2D backgroundTexture;
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

    const Vector3 MIDDLEGROUND_POS = {0.0f, 0.0f, 1.0f};
    const Vector3 BACKGROUND_POS = {0.0f, 0.0f, 0.0f};
    // Billboard Textures
    RenderTexture2D background;
    RenderTexture2D middleground;
    RenderTexture2D foreground;

    PlanetSelector planetSelector;
};

#endif // GAME_H