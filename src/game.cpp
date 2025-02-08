#include "../include/game.h"
#include "../include/Globals.h"

#define GLSL_VERSION 330

Game::Game()
{
    printf("Game Initialized!\n");
}

void Game::initialize()
{
    initializeShaders();

    player.initialize();
    // Sprites
    background = LoadTexture("resources/Art/background.png");
    enemy = LoadTexture("resources/Art/enemy.png");
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

    player.update(controller.getLeftStickDir());

    for (int i = 0; i < currentObjectAmount; i++)
    {
        objects[i]->update();
    }
}

void Game::draw() 
{
    DrawText("Hello, Raylib Starter Kit!", 190, 180, 20, DARKBLUE);

    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);
        DrawTextureEx(enemy, {100, 100}, 0, 0.1, WHITE);
        for (int i = 0; i < currentObjectAmount; i++)
        {
            objects[i]->draw();
        }
        player.draw();
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

        // Draw the background and unaffected elements normally
        DrawTextureEx(background, {0, 0}, 0, 1.0, WHITE);
    
        // First, draw the normal scene
        BeginBlendMode(BLEND_ALPHA);
            DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();

        // Then, add the glow effect on top using additive blending
        BeginBlendMode(BLEND_ADDITIVE);
            DrawTextureRec(targetBlur2.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();
    EndShaderMode();



    // Controller cursor
    if (IsGamepadAvailable(0))
    {
        controller.drawCursor();
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
        player.releaseGrapple(GetMousePosition());
    }

    // Used to grab objects
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        for (int i = 0; i < currentObjectAmount; i++)
        {
            if (CheckCollisionPointCircle(GetMousePosition(), objects[i]->getPos(), objects[i]->getRadius()))
            {
                player.shootGrapple(objects[i]);
            }
        }
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        objects.push_back(std::make_shared<Object>(GetMousePosition()));
        currentObjectAmount++;
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple(GetMousePosition());
    }

    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        for (int i = 0; i < currentObjectAmount; i++)
        {
            if (CheckCollisionPointCircle(controller.getCursorPos(), objects[i]->getPos(), objects[i]->getRadius()))
            {
                player.shootGrapple(objects[i]);
            }
        }
    }

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        objects.push_back(std::make_shared<Object>(controller.getCursorPos()));
        currentObjectAmount++;
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