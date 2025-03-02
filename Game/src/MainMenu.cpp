#include "../include/MainMenu.h"
#include "../include/Globals.h"
#include "../include/SceneCamera.h"
#include <memory>

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

    // Sounds
    buttonBreak = LoadSound("resources/Sound/asteroidBreak.wav");
    SetSoundVolume(buttonBreak, 0.1f);

    buttonGrabbed = LoadSound("resources/Sound/asteroidPierced.wav");
    SetSoundVolume(buttonGrabbed, 0.2f);

    Vector2 playPos = { SCREEN_WIDTH  / 2.0f, 500.0f };
    Vector2 exitPos = { SCREEN_WIDTH  / 2.0f, 700.0f };

    player.initialize();

    // Setup Buttons
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, playTexture, playPos, 50));
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, exitTexture, exitPos, 50));
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
    if (IsGamepadAvailable(0))
    {
        controllerInput();
    }
    else
    {
        mouseInput();
    }
}

void MainMenu::findClosestButton()
{
    float lowestDist = 100000.0f;

    for (std::shared_ptr<Button>& button : options)
    {
        float distToButton = pointToPointDist(button->getPos(), player.getPos());
        if (distToButton < lowestDist && !button->checkGrabbed() && button->isActive() && !button->checkToPlanet())
        {
            lowestDist = distToButton;
            closestButtonToPlayer = button;
        }
    }
}

bool MainMenu::checkIfAnOptionPickedup()
{
    for (std::shared_ptr<Button>& button : options)
    {
        if (button->checkGrabbed())
        {
            return true;
        }
    }

    return false;
}

void MainMenu::mouseInput()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        player.releaseGrapple(GetMousePosition(), false);
    } 
    
    // Used to grab Buttons
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (closestButtonToPlayer != nullptr && !checkIfAnOptionPickedup())
        {
            player.shootGrapple(closestButtonToPlayer);
        }
    }
}

void MainMenu::controllerInput()
{
}


void MainMenu::update()
{
    input();

    findClosestButton();

    player.update(controller.getLeftStickDir(), controller.getCursorPos());

    for (std::shared_ptr<Button>& button : options)
    {
        button->update();
    }
}


void MainMenu::draw()
{
    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);

        DrawTextureEx(logoTexture, {0, 100}, 0, 1.0, WHITE);

        for (std::shared_ptr<Button>& button : options)
        {
            button->draw();
        }

        player.draw();

        // Controller cursor
        if (IsGamepadAvailable(0))
        {
            controller.drawCursor();
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