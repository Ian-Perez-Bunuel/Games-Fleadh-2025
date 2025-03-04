#include "../include/game.h"
#include "../include/Globals.h"
#include "../include/DifficultyManager.h"
#include "../include/Transition.h"


Game::Game()
{
    printf("Game Initialized!\n");

    srand(time(nullptr));
}

void Game::initialize()
{
    initializeShaders();

    // Initialize all achievements
    achievementManager.init();
    AchievementManager::addGoalToAchievement("Hello World!", &amountOfClicks, 0);

    player.initialize({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f});
    player.initAchievements();
    player.turnOnHpBar();


    objectManager = std::make_unique<ObjectManager>(player);
    // Sprites
    for (int i = 0; i < 2; i ++)
    {
        backgroundTexture[i] = LoadTexture("resources/Art/2D/background.png");
    }
    backgroundPos[0] = {0, 0};
    backgroundPos[1] = {(float)backgroundTexture[1].width, 0};
    // astroidBeltTexture = LoadTexture("resources/Art/2D/bgAsteroids.png");

    reticleIn = LoadModel("resources/Art/3D/gyroscope1.glb");
    reticleMiddle = LoadModel("resources/Art/3D/gyroscope2.glb");
    reticleOut = LoadModel("resources/Art/3D/gyroscope3.glb");

    planetManager.init();


    // Camera initializing
    SceneCamera::initialize();

    // Music
    musicStart = LoadMusicStream("resources/Sound/themeIntro.wav");
    musicLoop = LoadMusicStream("resources/Sound/themeLoop.wav");
    PlayMusicStream(musicStart);
    SetMusicVolume(musicStart, musicVolume);
    SetMusicVolume(musicLoop, musicVolume);
    Transition::initSound();

    DifficultyManager::initBaseDifficulties();
    // SET DIFFICULTY TEMP
    DifficultyManager::setDifficulty(DifficultyManager::getDifficulty(1));
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

    // Particles for core collection
    collectingParticles.addColor(YELLOW);
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
    if (player.respawn())
    {
        planetManager.reset();
        objectManager->reset();
        AchievementManager::lockAll();
    }

    if (transitionedFromMenu)
    {
        transitionedFromMenu = false;
        Transition::begin();
    }

    if (GetMusicTimePlayed(musicStart) > GetMusicTimeLength(musicStart) - 1 && !musicLooping)
    {
        musicLooping = true;
        // Play the Intro music
        StopMusicStream(musicStart);
        PlayMusicStream(musicLoop);
    }

    if (musicLooping)
    {
        UpdateMusicStream(musicLoop); 
    }
    else
    {
        UpdateMusicStream(musicStart); 
    }

    // Achievements
    AchievementManager::checkForChanges();

    if (Transition::isActive())
    {
        objectManager->reset();

        Transition::update();
    }
    else
    {
        input();
        SceneCamera::update();
        
        player.update(controller.getLeftStickDir(), controller.getCursorPos());
        
        objectManager->update(planetManager.getMainPlanet());

        for (Projectile& proj : projectiles)
        {
            proj.update();
        } 

        if (planetManager.update(convertToMiddleCoords(player.getPos()), player))
        {
            objectManager->reset();
        }


        closestObjectToPlayer = objectManager->findClosestToPlayer();
    }
}

void Game::draw() 
{    
    drawMiddleground();
    drawBackground();

    BeginMode3D(SceneCamera::camera);
        DrawBillboard(SceneCamera::camera, background.texture, BACKGROUND_POS, 450.0f, WHITE);

        planetManager.drawOtherPlanets();
        
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

void Game::animateReticle()
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

void Game::drawMiddleground()
{
    // First Pass : Get starting scene
    BeginTextureMode(targetScene);
        ClearBackground(BLANK);

        BeginMode3D(SceneCamera::camera);
            if (!Transition::isActive())
            {
                planetManager.drawMainPlanet();

                if (closestObjectToPlayer != nullptr)
                {
                    animateReticle();
                    
                    DrawModel(reticleIn, convertToMiddleCoords(closestObjectToPlayer->getPos()), 0.25f, WHITE);
                    DrawModel(reticleMiddle, convertToMiddleCoords(closestObjectToPlayer->getPos()), 0.25f, WHITE);
                    DrawModel(reticleOut, convertToMiddleCoords(closestObjectToPlayer->getPos()), 0.25f, WHITE);
                }
            }

            player.draw3D();
        EndMode3D();

        planetManager.getMainPlanet().drawParticles();

        objectManager->draw();
        
        player.draw();

        achievementManager.draw();

        Transition::draw();

    EndTextureMode();


    // Second Pass : Apply Horizontal Blur
    BeginTextureMode(targetBlur1);
        ClearBackground(BLANK);
        BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
        BeginShaderMode(blurHorizontal);
            // Removed the negative sign on SCREEN_HEIGHT
            DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndShaderMode();
        EndBlendMode();
    EndTextureMode();

    // Third Pass: Apply Vertical Blur
    BeginTextureMode(targetBlur2);
        ClearBackground(BLANK);
        BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
        BeginShaderMode(blurVertical);
            // Changed negative height to positive
            DrawTextureRec(targetBlur1.texture, {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT}, {0, 0}, WHITE);
        EndShaderMode();
        EndBlendMode();
    EndTextureMode();

    BeginTextureMode(middleground);
        // Final Pass: Combine Glow with Original Scene
        BeginShaderMode(combineShader);
            ClearBackground(BLANK);
        
            // First, draw the normal scene
            BeginBlendMode(BLEND_ALPHA);
                // Use positive height
                DrawTextureRec(targetScene.texture, {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT}, {0, 0}, WHITE);
            EndBlendMode();

            // Then, add the glow effect on top using additive blending
            BeginBlendMode(BLEND_ADDITIVE);
                // Again, change to positive height
                DrawTextureRec(targetBlur2.texture, {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT}, {0, 0}, WHITE);
            EndBlendMode();
        EndShaderMode();
    EndTextureMode();

}

void Game::moveBackground()
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

void Game::drawBackground()
{
    BeginTextureMode(background);
        moveBackground();
        for (int i = 0; i < 2; i ++)
        {
            DrawTextureEx(backgroundTexture[i], backgroundPos[i], 0, 1.0, WHITE);
        }
        // DrawTextureEx(astroidBeltTexture, {0, 0}, 0, 1.0, BLUE );
    EndTextureMode();
}

void Game::coreCollection()
{
    collectingParticles.setValues({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, 0, 0);
    collectingParticles.circularSpawn(5, 150);
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
    if (IsKeyPressed(KEY_ESCAPE))
    {
        Transition::begin();
        SceneCamera::currentScene = Scene::MAIN_MENU;
        planetManager.reset();
        Player::resetStages();
        player.dropEverything();
        player.resetAchievements();
        objectManager->reset();
        AchievementManager::lockAll();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        player.releaseGrapple(GetMousePosition(), isMouseOverSphere(SceneCamera::camera, GetMousePosition(), planetManager.getMainPlanet().getPos(), 2));

        amountOfClicks++;
    }
    
    // Used to grab objects
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (closestObjectToPlayer != nullptr)
        {
            player.shootGrapple(closestObjectToPlayer);
        }
    }

    if (IsKeyPressed(KEY_UP))
    {
        Player::increase3DStage();
    }
}

void Game::controllerInput()
{
    controller.getAllInputs();

    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_LEFT))
    {
        Transition::begin();
        SceneCamera::currentScene = Scene::MAIN_MENU;
        planetManager.reset();
        player.dropEverything();
        player.resetAchievements();
        objectManager->reset();
        Player::resetStages();
    }
    
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2))
    {
        player.releaseGrapple(controller.getCursorPos(), isMouseOverSphere(SceneCamera::camera, controller.getCursorPos(), planetManager.getMainPlanet().getPos(), 2));
    }
    
    // Used to grab objects
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        if (closestObjectToPlayer != nullptr)
        {
            player.shootGrapple(closestObjectToPlayer);
        }
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
    
    return {normalizedX * SCREEN_BOUNDS_X, -normalizedY * SCREEN_BOUNDS_Y, MIDDLEGROUND_POS.z};
}

Vector2 Game::convertToGameCoords(Vector3 t_originalCoords)
{
    float normalizedX = Normalize(t_originalCoords.x, -SCREEN_BOUNDS_X, SCREEN_BOUNDS_X);
    float normalizedY = Normalize(t_originalCoords.y, -SCREEN_BOUNDS_Y, SCREEN_BOUNDS_Y);
    
    return {normalizedX * SCREEN_WIDTH, (normalizedY * SCREEN_HEIGHT)};
}




// Function that checks if the mouse is over a sphere in 3D space
bool Game::isMouseOverSphere(Camera camera, Vector2 mousePos, Vector3 spherePos, float sphereRadius) 
{
    // Create a ray from the current mouse position
    Ray ray = GetMouseRay(mousePos, camera);
    
    // Check if the ray intersects the sphere
    return CheckCollisionRaySphere(ray, spherePos, sphereRadius);
}

// Custom function to check for ray-sphere intersection
bool Game::CheckCollisionRaySphere(Ray ray, Vector3 spherePos, float sphereRadius) 
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