#include "../include/game.h"
#include "../include/Globals.h"

#define GLSL_VERSION 330

Game::Game()
{
    printf("Game Initialized!\n");
}

void Game::initialize()
{
    // Shader
	blurHorizontal = LoadShader(0, "resources/Shaders/glowHorizontal.fs");
    blurVertical = LoadShader(0, "resources/Shaders/glowVertical.fs");
    combineShader = LoadShader(0, "resources/Shaders/combine.fs");
    // textures
    targetScene = LoadRenderTexture(SCREEN_SIZE, SCREEN_SIZE);
    targetBlur1 = LoadRenderTexture(SCREEN_SIZE, SCREEN_SIZE);
    targetBlur2 = LoadRenderTexture(SCREEN_SIZE, SCREEN_SIZE);

    float resolution = SCREEN_SIZE  *2;

    // Uniforms
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);
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
        for (int i = 0; i < currentObjectAmount; i++)
        {
            objects[i]->draw();
        }
        player.draw();
    EndTextureMode();

    // Second Pass : Apply Horizontal Blur
    BeginTextureMode(targetBlur1);
        BeginShaderMode(blurHorizontal);
            DrawTextureRec(targetScene.texture, {0, 0, SCREEN_SIZE, -SCREEN_SIZE}, {0, 0}, WHITE);
        EndShaderMode();
    EndTextureMode();

    // Third Pass: Apply Vertical Blur
    BeginTextureMode(targetBlur2);
        BeginShaderMode(blurVertical);
            DrawTextureRec(targetBlur1.texture, {0, 0, SCREEN_SIZE, -SCREEN_SIZE}, {0, 0}, WHITE);
        EndShaderMode();
    EndTextureMode();

    // Final Pass: Combine Glow with Original Scene
    BeginShaderMode(combineShader);
        DrawTextureRec(targetScene.texture, {0, 0, SCREEN_SIZE, -SCREEN_SIZE}, {0, 0}, WHITE);
        BeginBlendMode(BLEND_ADDITIVE);
            DrawTextureRec(targetBlur2.texture, {0, 0, SCREEN_SIZE, -SCREEN_SIZE}, {0, 0}, WHITE);
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

    if (IsKeyPressed(KEY_UP))
    {
        glowRadius++;
        SetShaderValue(blurHorizontal, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        glowRadius--;
        SetShaderValue(blurHorizontal, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);
    }

    if (IsKeyPressed(KEY_LEFT))
    {
        glowIntensity--;
        SetShaderValue(combineShader, GetShaderLocation(combineShader, "intensity"), &glowIntensity, SHADER_UNIFORM_FLOAT);
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        glowIntensity++;
        SetShaderValue(combineShader, GetShaderLocation(combineShader, "intensity"), &glowIntensity, SHADER_UNIFORM_FLOAT);
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