#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include "../include/Button.h"
#include "../include/Player.h"
#include "../include/Controller.h"



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
    Controller controller;

    Texture2D backgroundTexture;

    Texture2D logoTexture;

    const float MAX_BUTTON_SCALE = 1.1f;

    Texture2D playTexture;
    Texture2D exitTexture;

    bool checkIfAnOptionPickedup();
    std::vector<std::shared_ptr<Button>> options;
    std::shared_ptr<Button> closestButtonToPlayer;

    Sound buttonBreak;
    Sound buttonGrabbed;

    void findClosestButton();
    Player player;
    

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


    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};

#endif // MAINMENU_H