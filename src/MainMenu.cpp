#include "../include/MainMenu.h"
#include <memory>
#include "../include/Globals.h"

Planet MainMenu::planet;
std::vector<std::shared_ptr<Button>> MainMenu::options;

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
    UnloadTexture(logoTexture);
    UnloadTexture(playTexture);
    UnloadTexture(exitTexture);

    UnloadTexture(easyTexture);
    UnloadTexture(mediumTexture);
    UnloadTexture(hardTexture);
    UnloadTexture(extremeTexture);

    UnloadTexture(arrow);
}

void MainMenu::initialize()
{
    backgroundTexture = LoadTexture("resources/Art/2D/background.png");
    logoTexture = LoadTexture("resources/Art/2D/logo.png");
    playTexture = LoadTexture("resources/Art/2D/play.png");
    exitTexture = LoadTexture("resources/Art/2D/exit.png");
    arrow = LoadTexture("resources/Art/2D/arrow.png");
    easyTexture = LoadTexture("resources/Art/2D/easy.png");
    mediumTexture = LoadTexture("resources/Art/2D/medium.png");
    hardTexture = LoadTexture("resources/Art/2D/hard.png");
    extremeTexture = LoadTexture("resources/Art/2D/extreme.png");

    // Reticle
    reticle = LoadTexture("resources/Art/2D/gyro.png");
    frameRectReticle = { 0.0f, 0.0f, (float)reticle.width / 8, (float)reticle.height };

    initializeShaders();

    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(1)); 

    // Sounds
    buttonBreak = LoadSound("resources/Sound/asteroidBreak.wav");
    SetSoundVolume(buttonBreak, 0.1f);

    buttonGrabbed = LoadSound("resources/Sound/asteroidPierced.wav");
    SetSoundVolume(buttonGrabbed, 0.2f);

    playPos = { SCREEN_WIDTH  / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 50};
    exitPos = { SCREEN_WIDTH  / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 50 };

    easyPos = { 200, 500.0f };
    mediumPos = { 200, 600.0f };
    hardPos = { 200, 700.0f };
    extremePos = { 200, 800.0f };
    arrowPos = {SCREEN_WIDTH * 0.9f, 400};

    player.initialize();

    planetPos = {6.0, -3.0, MAIN_PLANET_Z  + 3};
    planet.init(planetPos, 1, RED);

    // Setup Buttons
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, playTexture, playPos, BUTTON_RADIUS, playEffect));
    options[0]->setBaseColor(BLUE);
    options[0]->setCanDamage();
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, exitTexture, exitPos, BUTTON_RADIUS, exitEffect));
    options[1]->setBaseColor(BLUE);
    options[1]->setCanDamage();
    // Difficulties
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, easyTexture, easyPos, BUTTON_RADIUS, easyEffect));
    options[2]->setBaseColor(GREEN);
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, mediumTexture, mediumPos, BUTTON_RADIUS, mediumEffect));
    options[3]->setBaseColor(ORANGE);
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, hardTexture, hardPos, BUTTON_RADIUS, hardEffect));
    options[4]->setBaseColor(RED);
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, extremeTexture, extremePos, BUTTON_RADIUS, extremeEffect));
    options[5]->setBaseColor(DARKPURPLE);

    for (int i = 0; i < 6; i++)
    {
        printf("\n\n\nScale: %f\n\n\n", options[i]->getScale());
    }
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

    for (int i = 0; i < 6; i++)
    {
        float distToButton = pointToPointDist(options[i]->getPos(), player.getPos());
        if (distToButton < lowestDist && !options[i]->checkGrabbed() && options[i]->isActive() && !options[i]->checkToPlanet())
        {
            lowestDist = distToButton;
            closestButtonToPlayer = options[i];
        }
    }
}

bool MainMenu::checkIfAnOptionPickedup()
{
    for (int i = 0; i < 6; i++)
    {
        if (options[i]->checkGrabbed())
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
        player.releaseGrapple(GetMousePosition(), checkIfMouseOnPlanet(GetMousePosition(), planetPos, 2));

        buttonPickedUp = false;
    } 
    
    // Used to grab Buttons
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (closestButtonToPlayer != nullptr && !checkIfAnOptionPickedup())
        {
            player.shootGrapple(closestButtonToPlayer);

            buttonPickedUp = true;
        }
    }
}

void MainMenu::controllerInput()
{
    controller.getAllInputs();
    
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple(controller.getCursorPos(), checkIfMouseOnPlanet(controller.getCursorPos(), planetPos, 2));
    }
    
    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        if (closestButtonToPlayer != nullptr)
        {
            player.shootGrapple(closestButtonToPlayer);
        }
    }
}


void MainMenu::update()
{
    input();

    findClosestButton();

    player.update(controller.getLeftStickDir(), controller.getCursorPos());

    for (int i = 0; i < 6; i++)
    {
        options[i]->update();

        if (options[i]->checkToPlanet())
        {
            options[i]->movementToPlanet(planet);
        }
    }

    planet.update(convertToMiddleCoords(player.getPos()), player);

    // Activate specific effect
    if (planet.isCoreConsumed())
    {
        for (int i = 0; i < 6; i++)
        {
            if (!options[i]->isActive() && planet.checkIfParticlesActive())
            {
                if (options[i]->effect != nullptr)
                {
                    options[i]->effect();
                }
            }
        }
    }
}


void MainMenu::draw()
{
    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);

        for (int i = 0; i < 6; i++)
        {
            options[i]->draw();
        }

        player.draw();

        if (buttonPickedUp)
        {
            DrawTextureEx(arrow, arrowPos, 45, 1.0f, RED);
        }

        BeginMode3D(SceneCamera::camera);
            planet.draw();
            // DrawSphere(planetPos, 2, RED);
        EndMode3D();

        planet.drawParticles();

        if (closestButtonToPlayer != nullptr)
        {
            animateReticle();
            // Calculate destination rectangle with scaled width and height
            Rectangle destRec = {
                closestButtonToPlayer->getPos().x + 50,
                closestButtonToPlayer->getPos().y,
                frameRectReticle.width * 0.5f,   // scaled width
                frameRectReticle.height * 0.5f   // scaled height
            };

            DrawTexturePro(reticle, frameRectReticle, destRec, {destRec.width / 2.0f, destRec.height / 2.0f}, 45.0f, WHITE);
        }

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

    DrawTextureEx(logoTexture, {0, 100}, 0, 1.0, WHITE);
}

void MainMenu::animateReticle()
{
    framesCounter++;

    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;
        currentFrameReticle++;

        if (currentFrameReticle > 5) 
        {
            currentFrameReticle = 0;
        }

        frameRectReticle.x = (float)currentFrameReticle * (float)reticle.width / 8;
    }
}

Vector3 MainMenu::convertToMiddleCoords(Vector2 t_originalCoords)
{
    float normalizedX = normalizeSigned(t_originalCoords.x, 0.0f, SCREEN_WIDTH);
    float normalizedY = normalizeSigned(t_originalCoords.y, 0.0f, SCREEN_HEIGHT);
    
    return {normalizedX * SCREEN_BOUNDS_X, -normalizedY * SCREEN_BOUNDS_Y, MIDDLEGROUND_POS.z};
}

bool MainMenu::CheckCollisionRaySphere(Ray ray, Vector3 spherePos, float sphereRadius)
{
    // Compute vector from sphere center to ray origin
    Vector3 m = Vector3Subtract(ray.position, spherePos);

    // b is the projection of m onto the ray direction
    float b = Vector3Dot(m, ray.direction);
    // c is the squared distance from sphere center to the ray, minus radius squared
    float c = Vector3Dot(m, m) - sphereRadius * sphereRadius;

    // If the ray's origin is outside the sphere (c > 0)
    // and pointing away from the sphere (b > 0), there's no intersection.
    if (c > 0.0f && b > 0.0f)
        return false;

    // Calculate the discriminant of the quadratic equation
    float discriminant = b * b - c;
    if (discriminant < 0.0f)
        return false;  // No real roots, hence no intersection

    return true;  // The ray intersects the sphere
}

bool MainMenu::checkIfMouseOnPlanet(Vector2 mousePos, Vector3 spherePos, float sphereRadius)
{
    // Create a ray from the current mouse position
    Ray ray = GetMouseRay(mousePos, SceneCamera::camera);
    
    // Check if the ray intersects the sphere
    return CheckCollisionRaySphere(ray, spherePos, sphereRadius);
}


void MainMenu::playEffect()
{
    SceneCamera::currentScene = Scene::GAME;

    for (int i = 0; i < 6; i++)
    {
        options[i]->reset();
    }
}

void MainMenu::easyEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(0)); 
    planet.changeColor(GREEN);
}

void MainMenu::mediumEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(1)); 
    planet.changeColor(ORANGE);
}

void MainMenu::hardEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(2)); 
    planet.changeColor(RED);
}

void MainMenu::extremeEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(3)); 
    planet.changeColor(DARKPURPLE);
}