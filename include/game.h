#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <vector>
#include <memory>

#include "../include/Controller.h"
#include "../include/SceneCamera.h"
#include "../include/ObjectManager.h"
#include "../include/PlanetManager.h"
#include "../include/AchievementManager.h"


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

    void drawMiddleground();
    void drawBackground();

    void input();
    void mouseInput();
    void controllerInput();
    Controller controller;

    // Achievements
    AchievementManager achievementManager;
    int amountOfClicks = 0;
    
    Player player;
    std::shared_ptr<Object> closestObjectToPlayer;

    // Reticle
    void animateReticle();
    const float ROTATION_SPEED = 0.8f;
    float tilt = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    Model reticleIn;
    Model reticleMiddle;
    Model reticleOut;

    
    bool CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2);

    std::unique_ptr<ObjectManager> objectManager;

    void moveBackground();
    Vector2 backgroundPos[2];
    Texture2D backgroundTexture[2];
    Texture2D astroidBeltTexture;


    // Shaders
    float glowRadius = 15.0f;
    float glowIntensity = 1.0f;
    Shader blurHorizontal;
    Shader blurVertical;
    Shader combineShader;
    // Render textures for multi pass shaders
    RenderTexture2D targetScene;
    RenderTexture2D targetBlur1;
    RenderTexture2D targetBlur2;
    
    // Billboard Textures
    RenderTexture2D background;
    RenderTexture2D middleground;
    RenderTexture2D foreground;


    // Projectile testing
    std::vector<Projectile> projectiles;

    // Planet
    void coreCollection();
    bool collectingCore = false;
    ParticleSpawner collectingParticles;
    PlanetManager planetManager;

    Vector2 mousePos;

    // Music
    Music musicStart;
    Music musicLoop;
    bool musicLooping = false;
    float musicVolume = 0.7f; 

    bool transitionedFromMenu = true;

    // Normalize to -1 to 1 range
    float normalizeSigned(float x, float t_min, float t_max) 
    {
        return 2 * (x - t_min) / (t_max - t_min) - 1;
    }
    Vector3 convertToMiddleCoords(Vector2 t_originalCoords);
    Vector2 convertToGameCoords(Vector3 t_originalCoords);

    float Vector3Dot(Vector3 v1, Vector3 v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
    bool CheckCollisionRaySphere(Ray ray, Vector3 spherePos, float sphereRadius);
    bool isMouseOverSphere(Camera camera, Vector2 mousePos, Vector3 spherePos, float sphereRadius);
};

#endif // GAME_H