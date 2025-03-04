#include "../include/MainMenu.h"
#include <memory>
#include "../include/Globals.h"
#include "../include/Transition.h"

Planet MainMenu::planet;
std::vector<std::shared_ptr<Button>> MainMenu::options;

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
    UnloadTexture(logoTexture);
    UnloadTexture(exitTexture);

    UnloadTexture(easyTexture);
    UnloadTexture(mediumTexture);
    UnloadTexture(hardTexture);
    UnloadTexture(extremeTexture);
}

void MainMenu::initialize()
{
    controller.init();

    for (int i = 0; i < 2; i ++)
    {
        backgroundTexture[i] = LoadTexture("resources/Art/2D/background.png");
    }
    backgroundPos[0] = {0, 0};
    backgroundPos[1] = {(float)backgroundTexture[1].width, 0};

    logoTexture = LoadTexture("resources/Art/2D/logo.png");
    exitTexture = LoadTexture("resources/Art/2D/exit.png");
    easyTexture = LoadTexture("resources/Art/2D/easy.png");
    mediumTexture = LoadTexture("resources/Art/2D/medium.png");
    hardTexture = LoadTexture("resources/Art/2D/hard.png");
    extremeTexture = LoadTexture("resources/Art/2D/extreme.png");

    // Reticle
    reticleIn = LoadModel("resources/Art/3D/gyroscope1.glb");
    reticleMiddle = LoadModel("resources/Art/3D/gyroscope2.glb");
    reticleOut = LoadModel("resources/Art/3D/gyroscope3.glb");

    initializeShaders();

    // Sounds
    buttonBreak = LoadSound("resources/Sound/asteroidBreak.wav");
    SetSoundVolume(buttonBreak, 0.1f);

    buttonGrabbed = LoadSound("resources/Sound/asteroidPierced.wav");
    SetSoundVolume(buttonGrabbed, 0.2f);

    exitPos = { SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.95f };

    easyPos = { 250, SCREEN_HEIGHT * 0.45f };
    mediumPos = { 250, SCREEN_HEIGHT * 0.6f };
    hardPos = { 250, SCREEN_HEIGHT * 0.75f };
    extremePos = { 250, SCREEN_HEIGHT * 0.9f };

    player.initialize({SCREEN_WIDTH / 2.0f - 30, SCREEN_HEIGHT * 0.45f});

    planetPos = {0.0, -1.25, MAIN_PLANET_Z + 8};
    planet.init(planetPos, 1, RED);

    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(1)); 

    // Setup Buttons
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, exitTexture, exitPos, BUTTON_RADIUS, 50, exitEffect));
    options[0]->setBaseColor(BLUE);
    // Difficulties
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, easyTexture, easyPos, BUTTON_RADIUS, 50, easyEffect));
    options[1]->setBaseColor(GREEN);
    options[1]->setCanDamage();
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, mediumTexture, mediumPos, BUTTON_RADIUS, 135, mediumEffect));
    options[2]->setBaseColor(ORANGE);
    options[2]->setCanDamage();
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, hardTexture, hardPos, BUTTON_RADIUS, 50, hardEffect));
    options[3]->setBaseColor(RED);
    options[3]->setCanDamage();
    options.push_back(std::make_shared<Button>(buttonBreak, buttonGrabbed, extremeTexture, extremePos, BUTTON_RADIUS, 175, extremeEffect));
    options[4]->setBaseColor(DARKPURPLE);
    options[4]->setCanDamage();
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

    for (int i = 0; i < MAX_OPTIONS; i++)
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
    for (int i = 0; i < MAX_OPTIONS; i++)
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
    
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        player.releaseGrapple(controller.getCursorPos(), checkIfMouseOnPlanet(controller.getCursorPos(), planetPos, 2));

        buttonPickedUp = false;
    }
    
    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {
        if (closestButtonToPlayer != nullptr && !checkIfAnOptionPickedup())
        {
            player.shootGrapple(closestButtonToPlayer);

            buttonPickedUp = true;
        }
    }
}


void MainMenu::update()
{
    input();

    if (Transition::isActive())
    {
        Transition::update();
    }

    findClosestButton();

    player.update(controller.getLeftStickDir(), controller.getCursorPos());

    for (int i = 0; i < MAX_OPTIONS; i++)
    {
        options[i]->update();

        if (options[i]->checkToPlanet())
        {
            options[i]->movementToPlanet(planet);
        }

        if (options[i]->getScale() <= 0.0f)
        {
            options[i]->reset();
        }
    }

    planet.update(convertToMiddleCoords(player.getPos()), player);

    // Activate specific effect
    for (int i = 0; i < MAX_OPTIONS; i++)
    {
        if (!options[i]->checkIfCanDamage())
        {
            if (!options[i]->isActive())
            {
                if (options[i]->effect != nullptr)
                {
                    options[i]->effect();
                }

                options[i]->reset();
            }
        }
        else if (planet.isCoreConsumed())
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

        for (int i = 0; i < MAX_OPTIONS; i++)
        {
            options[i]->draw();
        }

        BeginMode3D(SceneCamera::camera);
            planet.draw();
            
            if (closestButtonToPlayer != nullptr)
            {
                animateReticle();
                
                Vector3 reticlePos = convertToMiddleCoords(closestButtonToPlayer->getPos());
                reticlePos.x += 0.4f;

                DrawModelWires(reticleIn, reticlePos, 0.25f, WHITE);
                DrawModelWires(reticleMiddle, reticlePos, 0.25f, WHITE);
                DrawModelWires(reticleOut, reticlePos, 0.25f, WHITE);
            }

            // Controller cursor
            if (IsGamepadAvailable(0) && checkIfAnOptionPickedup())
            {
                controller.drawCursor();
            }

        EndMode3D();

        player.draw();

        for (int i = 0; i < MAX_OPTIONS; i++)
        {
            options[i]->draw();
        }


        planet.drawParticles();
        
        Transition::draw();

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
        moveBackground();
        for (int i = 0; i < 2; i ++)
        {
            DrawTextureEx(backgroundTexture[i], backgroundPos[i], 0, 1.0, WHITE);
        }

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

void MainMenu::moveBackground()
{
    for (int i = 0; i < 2; i++)
    {
        backgroundPos[i].x -= 0.5f;

        if (backgroundPos[i].x <= -backgroundTexture[i].width)
        {
            backgroundPos[i].x = backgroundTexture[i].width - 10;
        }
    }
}

void MainMenu::animateReticle()
{
    tilt += ROTATION_SPEED;
    pitch += ROTATION_SPEED;
    roll += ROTATION_SPEED;
    yaw += ROTATION_SPEED;
    // Rotate gyro
    Matrix tiltMatrixOut = MatrixRotateY(DEG2RAD * tilt);
    Matrix spinMatrixOut = MatrixRotateX(DEG2RAD * pitch);
    Matrix tiltMatrixMiddle = MatrixRotateX(DEG2RAD * tilt);
    Matrix spinMatrixMiddle = MatrixRotateY(DEG2RAD * roll);
    Matrix tiltMatrixIn = MatrixRotateY(DEG2RAD * tilt);
    Matrix spinMatrixIn = MatrixRotateZ(DEG2RAD * yaw);

    // Combine the rotations
    Matrix rotationMatrixOut = MatrixMultiply(spinMatrixOut, tiltMatrixOut);
    Matrix rotationMatrixMiddle = MatrixMultiply(spinMatrixMiddle, tiltMatrixMiddle);
    Matrix rotationMatrixIn = MatrixMultiply(spinMatrixIn, tiltMatrixIn);

    // Apply the combined rotation to the planet models
    reticleOut.transform = rotationMatrixOut;
    reticleMiddle.transform = rotationMatrixMiddle;
    reticleIn.transform = rotationMatrixIn;
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

    for (int i = 0; i < MAX_OPTIONS; i++)
    {
        options[i]->reset();
        planet.setAlive();
    }
}

void MainMenu::easyEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(0)); 
    planet.changeColor(GREEN);

    ParticleSpawner& particles = planet.getParticles();
    particles.clearColors();
    particles.addColor(GREEN);

    playEffect();
}

void MainMenu::mediumEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(1)); 
    planet.changeColor(ORANGE);
    
    ParticleSpawner& particles = planet.getParticles();
    particles.clearColors();
    particles.addColor(ORANGE);

    playEffect();
}

void MainMenu::hardEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(2)); 
    planet.changeColor(RED);
    
    ParticleSpawner& particles = planet.getParticles();
    particles.clearColors();
    particles.addColor(RED);

    playEffect();
}

void MainMenu::extremeEffect()
{
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(3)); 
    planet.changeColor(DARKPURPLE);
    
    ParticleSpawner& particles = planet.getParticles();
    particles.clearColors();
    particles.addColor(DARKPURPLE);

    playEffect();
}