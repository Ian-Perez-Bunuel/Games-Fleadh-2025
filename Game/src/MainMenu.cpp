#include "../include/MainMenu.h"
#include "../include/Globals.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
    UnloadTexture(logoTexture);
    UnloadTexture(playTexture);
    UnloadTexture(exitTexture);
}

void MainMenu::initialize()
{
    backgroundTexture = LoadTexture("resources/Art/2D/background.png");
    logoTexture = LoadTexture("resources/Art/2D/logo.png");
    playTexture = LoadTexture("resources/Art/2D/play.png");
    exitTexture = LoadTexture("resources/Art/2D/exit.png");
    initializeShaders();

    // Setup Buttons
    playButton.setup({(SCREEN_WIDTH / 2.0f) - 25, (SCREEN_HEIGHT / 2.0f) + 125}, 300, 100);
    exitButton.setup({(SCREEN_WIDTH / 2.0f) - 25, (SCREEN_HEIGHT / 2.0f) + 330}, 225, 100);
}

void MainMenu::initializeShaders()
{
    // Shader
	blurHorizontal = LoadShader(0, "resources/Shaders/glowHorizontal.fs");
    blurVertical = LoadShader(0, "resources/Shaders/glowVertical.fs");
    combineShader = LoadShader(0, "resources/Shaders/combine.fs");
    // textures
    targetScene = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur1 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    targetBlur2 = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set texture filtering to smooth (GL_LINEAR)
    SetTextureFilter(targetScene.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(targetBlur2.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 resolutionForBlur = {(float)SCREEN_WIDTH * 5, (float)SCREEN_HEIGHT * 5};

    // Uniforms
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "resolution"), &resolutionForBlur.x, SHADER_UNIFORM_FLOAT);
    SetShaderValue(blurHorizontal, GetShaderLocation(blurHorizontal, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "resolution"), &resolutionForBlur.y, SHADER_UNIFORM_FLOAT); 
    SetShaderValue(blurVertical, GetShaderLocation(blurVertical, "radius"), &glowRadius, SHADER_UNIFORM_FLOAT);

    SetShaderValue(combineShader, GetShaderLocation(combineShader, "intensity"), &glowIntensity, SHADER_UNIFORM_FLOAT);
}

void MainMenu::input()
{
}

void MainMenu::mouseInput()
{
    if (playButton.checkForMouse())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            printf("\n\nON PLAY\n\n");
            currentScene = Scene::GAME;
            printf("SCENE: %d\n\n", currentScene);
        }

        if (playScale < MAX_BUTTON_SCALE)
        {
            playScale += 0.01f;
        }
    }
    else
    {
        if (playScale > 1)
        {
            playScale -= 0.01f;
        }
    }
    
    if (exitButton.checkForMouse())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            CloseWindow();
        }

        if (exitScale < MAX_BUTTON_SCALE)
        {
            exitScale += 0.01f;
        }
    }
    else
    {
        if (exitScale > 1)
        {
            exitScale -= 0.01f;
        }
    }
}

void MainMenu::controllerInput()
{
}


void MainMenu::update()
{
    mouseInput();
}


void MainMenu::draw()
{
    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);

        DrawTextureEx(logoTexture, {0, 0}, 0, 1.0, WHITE);

        DrawTextureEx(playTexture, {0, 0}, 0, playScale, WHITE);
        DrawTextureEx(exitTexture, {0, 0}, 0, exitScale, WHITE);

        exitButton.draw();
        playButton.draw();
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

    BeginShaderMode(combineShader);
        ClearBackground(BLANK);

        // Draw non-glow objects
        DrawTextureEx(backgroundTexture, {0, 0}, 0, 1.0, WHITE);

        // First, draw the normal scene
        BeginBlendMode(BLEND_ALPHA);
            DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();

        // Then, add the glow effect on top using additive blending
        BeginBlendMode(BLEND_ADDITIVE);
           DrawTextureRec(targetBlur2.texture, {0, 0, (float)SCREEN_WIDTH, (float)-SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndBlendMode();
    EndShaderMode();
}