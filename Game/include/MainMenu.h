#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include "../include/Button.h"



class MainMenu
{
public:
    MainMenu();
    ~MainMenu();

    void initialize();
    void update();
    void draw();

private:
    void initializeShaders();

    void input();
    void mouseInput();
    void controllerInput();

    Texture2D backgroundTexture;

    Texture2D logoTexture;

    const float MAX_BUTTON_SCALE = 1.1f;

    Texture2D playTexture;
    Button playButton;
    Vector2 playPos;
    float playScale = 1.0f;

    Texture2D exitTexture;
    Button exitButton;
    Vector2 exitPos;
    float exitScale = 1.0f;

    // Shaders
    float glowRadius = 15.0f;
    float glowIntensity = 0.1f;
    Shader blurHorizontal;
    Shader blurVertical;
    Shader combineShader;
    // Render textures for multi pass shaders
    RenderTexture2D targetScene;
    RenderTexture2D targetBlur1;
    RenderTexture2D targetBlur2;
    RenderTexture2D middleground;
};

#endif // MAINMENU_H