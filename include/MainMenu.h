#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <vector>
#include "../include/Button.h"
#include "../include/Player.h"
#include "../include/Planet.h"
#include "../include/Controller.h"
#include "../include/SceneCamera.h"
#include "../include/DifficultyManager.h"



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

    Texture2D arrow;
    Vector2 arrowPos;

    const float MAX_BUTTON_SCALE = 1.1f;

    Texture2D playTexture;
    Texture2D exitTexture;

    Texture2D easyTexture;
    Texture2D mediumTexture;
    Texture2D hardTexture;
    Texture2D extremeTexture;

    bool checkIfAnOptionPickedup();
    static std::vector<std::shared_ptr<Button>> options;
    std::shared_ptr<Button> closestButtonToPlayer;

    Sound buttonBreak;
    Sound buttonGrabbed;

    void findClosestButton();
    Player player;

    float Vector3Dot(Vector3 v1, Vector3 v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
    bool CheckCollisionRaySphere(Ray ray, Vector3 spherePos, float sphereRadius);
    bool checkIfMouseOnPlanet(Vector2 mousePos, Vector3 spherePos, float sphereRadius);
    static Planet planet;
    Vector3 planetPos;

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

    bool buttonPickedUp = false;

    // Reticle
    void animateReticle();
    Texture2D reticle;
    Rectangle frameRectReticle;
    int currentFrameReticle;
    int framesCounter = 0;
    int framesSpeed = 8;

    // Button variables
    Vector2 playPos;
    Vector2 exitPos;

    Vector2 easyPos;
    Vector2 mediumPos;
    Vector2 hardPos;
    Vector2 extremePos;
    const int BUTTON_RADIUS = 25;
    
    // Button effects
    static void playEffect();
    static void exitEffect() { CloseWindow(); }
    static void easyEffect();
    static void mediumEffect();
    static void hardEffect();
    static void extremeEffect();

    // Normalize to -1 to 1 range
    float normalizeSigned(float x, float t_min, float t_max) { return 2 * (x - t_min) / (t_max - t_min) - 1; }
    Vector3 convertToMiddleCoords(Vector2 t_originalCoords);
    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};

#endif // MAINMENU_H