#include "../include/game.h"
#include "../include/Globals.h"


Game::Game()
{
    printf("Game Initialized!\n");

    srand(time(nullptr));
}

void Game::initialize()
{
    initializeShaders();

    player.initialize();
    // Sprites
    background = LoadTexture("resources/Art/background.png");
    enemy = LoadTexture("resources/Art/Planet.png");

    reticle = LoadTexture("resources/Art/target.png");

    planetSelector.init();
    

    // Camera initializing
    SceneCamera::initialize();
}
void Game::initializeShaders()
{
    // Shader
	blurHorizontal = LoadShader(0, "resources/Shaders/glowHorizontal.fs");
    blurVertical = LoadShader(0, "resources/Shaders/glowVertical.fs");
    combineShader = LoadShader(0, "resources/Shaders/combine.fs");
    crtShader = LoadShader(0, "resources/Shaders/crt.fs");
    // textures
    targetScene = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur1 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur2 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set texture filtering to smooth (GL_LINEAR)
    SetTextureFilter(targetScene.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur2.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 resolutionForBlur = {(float)SCREEN_WIDTH * 5, (float)SCREEN_HEIGHT * 5};
    Vector2 resolutionForCRT = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};

    // Uniforms
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "resolution"), &resolutionForBlur.x, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "resolution"), &resolutionForBlur.y, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(combineShader, GetShaderLocation(combineShader, "intensity"), &glowIntensity, SHADER_UNIFORM_FLOAT);

    SetShaderValue(crtShader, GetShaderLocation(crtShader, "resolution"), &resolutionForCRT, SHADER_UNIFORM_FLOAT);
}

Game::~Game()
{
    // Use to free resources
    UnloadShader(blurHorizontal);
    UnloadShader(blurVertical);
    UnloadShader(combineShader);
    UnloadRenderTexture(targetScene);
    UnloadRenderTexture(targetBlur1);
    UnloadRenderTexture(targetBlur2);
}

void Game::update() 
{
    input();
    // SceneCamera::update();

    
    player.update(controller.getLeftStickDir());
    
    objectManager.update();

    if (planetSelector.isActive())
    {
        planetSelector.transition();
    }

    closestObjectToPlayer = objectManager.findClosestToPlayer(player);
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);

    
    drawWithGlow();


    // Controller cursor
    if (IsGamepadAvailable(0))
    {
        controller.drawCursor();
    }
}

void Game::drawWithGlow()
{
    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);

        objectManager.draw();
        
        player.draw();

        if (closestObjectToPlayer != nullptr)
        {
            DrawTextureEx(reticle, {closestObjectToPlayer->getPos().x - 25, closestObjectToPlayer->getPos().y - 25}, 0, 1.25f, WHITE);
        }

    EndTextureMode();


    // Second Pass : Apply Horizontal Blur
    BeginTextureMode(targetBlur1);
        ClearBackground(BLANK);
        BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
        BeginShaderMode(blurHorizontal);
            DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndShaderMode();
        EndBlendMode();
    EndTextureMode();

    // Third Pass: Apply Vertical Blur
    BeginTextureMode(targetBlur2);
        ClearBackground(BLANK);
        BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
        BeginShaderMode(blurVertical);
            DrawTextureRec(targetBlur1.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndShaderMode();
        EndBlendMode();
    EndTextureMode();

    // Final Pass: Combine Glow with Original Scene
    BeginShaderMode(combineShader);
        ClearBackground(BLANK);

        BeginMode2D(SceneCamera::camera);

        // Draw the background and unaffected elements normally
        drawWithoutGlow();
    
        // First, draw the normal scene
        BeginBlendMode(BLEND_ALPHA);
            DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();

        // Then, add the glow effect on top using additive blending
        BeginBlendMode(BLEND_ADDITIVE);
            DrawTextureRec(targetBlur2.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();

        EndMode2D();

    EndShaderMode();
}

void Game::drawWithoutGlow()
{
    DrawTextureEx(background, {0, 0}, 0, 1.0, WHITE);

    if (planetSelector.isActive())
    {
        planetSelector.draw();
    }
}

void Game::input()
{
    if (IsGamepadAvailable(0))
    {
        controllerInput();
    }
    else
    {
        mouseInput();
    }
}

void Game::mouseInput()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        player.releaseGrapple( GetScreenToWorld2D(GetMousePosition(), SceneCamera::camera));
    }

    // Used to grab objects
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        player.shootGrapple(closestObjectToPlayer);
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        objectManager.setObjects();
    }

    if (IsKeyPressed(KEY_M))
    {
        planetSelector.activate();
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple( GetScreenToWorld2D(GetMousePosition(), SceneCamera::camera));
    }

    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        player.shootGrapple(closestObjectToPlayer);
    }

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        objectManager.setObjects();
    }
}

bool Game::CircleCollisions(int t_r1, int t_r2, Vector2 pos1, Vector2 pos2)
{
    // Calculate the squared distance between the two positions
    double distanceSquared = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2);
    
    // Calculate the squared sum of the radii
    int radiusSum = t_r1 + t_r2;
    double radiusSumSquared = pow(radiusSum, 2);
    
    // Check if the distance squared is less than or equal to the squared radius sum
    return distanceSquared <= radiusSumSquared;
}