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
    backgroundTexture = LoadTexture("resources/Art/background.png");
    enemy = LoadTexture("resources/Art/Planet.png");

    reticle = LoadTexture("resources/Art/target.png");

    planetSelector.init();
    planet.init(PLANET_POS);

    // Camera initializing
    SceneCamera::initialize();
}
void Game::initializeShaders()
{
    // Shader
	blurHorizontal = LoadShader(0, "resources/Shaders/glowHorizontal.fs");
    blurVertical = LoadShader(0, "resources/Shaders/glowVertical.fs");
    combineShader = LoadShader(0, "resources/Shaders/combine.fs");
    // textures
    targetScene = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur1 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur2 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    // Scene parts
    background = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    middleground = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    foreground = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set texture filtering to smooth (GL_LINEAR)
    SetTextureFilter(targetScene.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur2.texture, TEXTURE_FILTER_BILINEAR);

    SetTextureFilter(background.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(middleground.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(foreground.texture, TEXTURE_FILTER_BILINEAR);


    

    Vector2 resolutionForBlur = {(float)SCREEN_WIDTH * 5, (float)SCREEN_HEIGHT * 5};

    // Uniforms
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "resolution"), &resolutionForBlur.x, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "resolution"), &resolutionForBlur.y, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(combineShader, GetShaderLocation(combineShader, "intensity"), &glowIntensity, SHADER_UNIFORM_FLOAT);
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
    SceneCamera::update();

    
    player.update(controller.getLeftStickDir());
    
    objectManager.update();

    if (planetSelector.isActive())
    {
        planetSelector.transition();
    }

    for (Projectile& proj : projectiles)
    {
        proj.update();
    } 

    planet.update();

    closestObjectToPlayer = objectManager.findClosestToPlayer(player);
}

void Game::draw() 
{    
    drawMiddleground();
    drawBackground();

    BeginMode3D(SceneCamera::camera);
        DrawBillboard(SceneCamera::camera, background.texture, BACKGROUND_POS, 25.0f, WHITE);

        planet.draw();
        for (Projectile& proj : projectiles)
        {
            proj.draw();
        }   

        DrawBillboard(SceneCamera::camera, middleground.texture, MIDDLEGROUND_POS, 7.8f, WHITE);
    EndMode3D();

    // Controller cursor
    if (IsGamepadAvailable(0))
    {
        controller.drawCursor();
    }
}

void Game::drawMiddleground()
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

    BeginTextureMode(middleground);
        // Final Pass: Combine Glow with Original Scene
        BeginShaderMode(combineShader);
            ClearBackground(BLANK);
    
            // First, draw the normal scene
            BeginBlendMode(BLEND_ALPHA);
                DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
            EndBlendMode();

            // Then, add the glow effect on top using additive blending
            BeginBlendMode(BLEND_ADDITIVE);
                DrawTextureRec(targetBlur2.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
            EndBlendMode();
            EndShaderMode();
    EndTextureMode();
}

void Game::drawBackground()
{
    BeginTextureMode(background);
        DrawTextureEx(backgroundTexture, {0, 0}, 0, 1.0, WHITE);
    EndTextureMode();

    if (planetSelector.isActive())
    {
        planetSelector.draw();
    }
}

void Game::input()
{
    mousePos = {GetMousePosition().x, SCREEN_HEIGHT - GetMousePosition().y};

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
        player.releaseGrapple(mousePos);
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

    if (IsKeyPressed(KEY_F))
    {
        projectiles.push_back(Projectile(PLANET_POS, convertToMiddleCoords(player.getPos())));
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple(mousePos);
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


Vector3 Game::convertToMiddleCoords(Vector2 t_originalCoords)
{
    float normalizedX = normalizeSigned(t_originalCoords.x, 0.0f, SCREEN_WIDTH);
    float normalizedY = normalizeSigned(t_originalCoords.y, 0.0f, SCREEN_HEIGHT);

    return {normalizedX * 7.05f, normalizedY * 3.8f, MIDDLEGROUND_POS.z};
}

Vector2 Game::convertToGameCoords(Vector2 t_originalCoords)
{
    float normalizedX = normalizeSigned(t_originalCoords.x, 0.0f, 7.05f);
    float normalizedY = normalizeSigned(t_originalCoords.y, 0.0f, 3.8f);

    return {normalizedX * SCREEN_WIDTH, normalizedY * SCREEN_HEIGHT};
}