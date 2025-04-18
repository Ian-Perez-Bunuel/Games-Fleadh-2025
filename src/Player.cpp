#include "../include/Player.h"
#include "../include/SceneCamera.h"
#include "../include/AchievementManager.h"
#include "../include/Globals.h"
#include "../include/Controller.h"
#include "../include/Transition.h"
#include "rlgl.h"

int Player::stage = 0;

Player::Player()
{
}

void Player::initialize(Vector2 t_pos)
{
    hullTexture = LoadTexture("resources/Art/2D/playerHull.png");
    beamTexture = LoadTexture("resources/Art/2D/playerBeams.png");
    ringTexture = LoadTexture("resources/Art/2D/playerRing.png");

    // Models
    printf("\n\n");
    ring = LoadModel("resources/Art/3D/playerRing.glb");
    beams = LoadModel("resources/Art/3D/playerBeams.glb");
    hull = LoadModel("resources/Art/3D/playerHull.glb");
    printf("\n\n");

    damageSound = LoadSound("resources/Sound/playerDamage.wav");
    SetSoundVolume(damageSound, 0.3f);
    deathSound = LoadSound("resources/Sound/playerDeath.wav");
    SetSoundVolume(damageSound, 0.45f);

    position = t_pos;

    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].init();
    }

    particles.addColor(YELLOW);
    particles.addColor(RED);

    initHealthBar();
}

void Player::initAchievements()
{
    // Object Achievements
    AchievementManager::addGoalToAchievement("Baby's first toy", &objectsGrabbed, 1);
    AchievementManager::addGoalToAchievement("GRAB, GRAB, GRAB!", &objectsGrabbed, 5);
    AchievementManager::addGoalToAchievement("ALL FULL!!!", &maxGrappledObjects, GRAPPLE_AMOUNT);
    AchievementManager::addGoalToAchievement("Asteroid Lover", &objectsGrabbed, 20);
    AchievementManager::addGoalToAchievement("Rock Collector", &objectsGrabbed, 50);

    // Movement Achievements
    AchievementManager::addGoalToAchievement("Engines On!", &movementDone, 5);
    AchievementManager::addGoalToAchievement("Marathon Runner", &movementDone, 1500);
}

void Player::rotateToMouse()
{
    rotation = atan2f(GetMousePosition().y  - position.y, GetMousePosition().x  - position.x) * RAD2DEG;
}

void Player::rotateToController(Vector2 t_cursorPos)
{
    rotation = atan2f(t_cursorPos.y - position.y, t_cursorPos.x - position.x) * RAD2DEG;
}

void Player::boundryChecking()
{
    if (position.x < RADIUS)
    {
        position.x = RADIUS;
    }
    else if (position.x > SCREEN_WIDTH - RADIUS)
    {
        position.x = SCREEN_WIDTH - RADIUS;
    }
    
    if (position.y < RADIUS)
    {
        position.y = RADIUS;
    }
    else if (position.y > SCREEN_HEIGHT - RADIUS)
    {
        position.y = SCREEN_HEIGHT - RADIUS;
    }
}

void Player::draw3D()
{
    if (alive)
    {
        if (stage >= 2)
        {
            DrawModelWires(ring, position3D, scale / 10.0f, color);
        }
        if (stage >= 4)
        {
            DrawModelWires(beams, position3D, scale / 10.0f, color);
        }
        if (stage >= 6)
        {
            DrawModelWires(hull, position3D, scale / 10.0f, color);
        }
    }
}

void Player::move()
{
    rotateToMouse();
    boundryChecking();

    // key inputs
    if (IsKeyDown(KEY_W))
    {
        velocity.y -= SPEED;
        movementDone++;
    }
    else if (IsKeyDown(KEY_S))
    {
        velocity.y += SPEED;
        movementDone++;
    } 
    if (IsKeyDown(KEY_A))
    {
        velocity.x -= SPEED;
        movementDone++;
    }
    else if (IsKeyDown(KEY_D))
    {
        velocity.x += SPEED;
        movementDone++;
    }

    // If no movement keys pressed
    if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
    {
        velocity.y *= FRICTION;
    }
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
    {
        velocity.x *= FRICTION;
    }
    
    // Make sure the player doesnt go too fast
    capSpeed();


    position.x += velocity.x;
    position.y += velocity.y;
}

void Player::controllerMovement(Vector2 t_stickDir, Vector2 t_cursorPos)
{
    rotateToController(t_cursorPos);
    boundryChecking();

    velocity.x += t_stickDir.x * 0.4f;
    velocity.y += t_stickDir.y * 0.4f;

    if (velocity.x >= 0.2f || velocity.y >= 0.2f)
    {
        movementDone++;
    }

    if (t_stickDir.x == 0 && t_stickDir.y == 0)
    {
        velocity.x *= FRICTION;
        velocity.y *= FRICTION;
    }

    // Make sure the player doesnt go too fast
    capSpeed();

    position.x += velocity.x;
    position.y += velocity.y;
}


void Player::capSpeed()
{
    if (velocity.x > MAX_SPEED)
    {
        velocity.x = MAX_SPEED;
    }
    else if (velocity.x < -MAX_SPEED)
    {
        velocity.x = -MAX_SPEED;
    }

    if (velocity.y > MAX_SPEED)
    {
        velocity.y = MAX_SPEED;
    }
    else if (velocity.y < -MAX_SPEED)
    {
        velocity.y = -MAX_SPEED;
    }
}

void Player::animation()
{
    scale = 1.0f + 0.05f * sin(GetTime() * 2.0f * PI);
}

void Player::draw()
{
    if (alive)
    {
        animation();

        for (int i = 0; i < GRAPPLE_AMOUNT; i++)
        {
            grapples[i].draw(); 
        }
        SetTextureFilter(hullTexture, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(beamTexture, TEXTURE_FILTER_BILINEAR);
        SetTextureFilter(ringTexture, TEXTURE_FILTER_BILINEAR);
        Rectangle destRec = { position.x, position.y, ringTexture.width * scale, ringTexture.height * scale };
        Vector2 origin = { hullTexture.width / 2.0f, hullTexture.height / 2.0f };

        if (stage <= 1)
        {
            Rectangle sourceRecRing = { 0, 0, (float)ringTexture.width, (float)ringTexture.height };
            DrawTexturePro(ringTexture, sourceRecRing, destRec, origin, rotation, color);
        }
        if (stage <= 3)
        {
            Rectangle sourceRecBeam = { 0, 0, (float)beamTexture.width, (float)beamTexture.height };
            DrawTexturePro(beamTexture, sourceRecBeam, destRec, origin, rotation, color);
        }
        if (stage <= 5)
        {
            Rectangle sourceRecHull = { 0, 0, (float)hullTexture.width, (float)hullTexture.height };
            DrawTexturePro(hullTexture, sourceRecHull, destRec, origin, rotation, color);
        }

        // HP
        drawHealthBar();
    }

    particles.draw();
}

void Player::shootGrapple(std::shared_ptr<Object> t_object)
{
    if (!t_object->checkGrabbed() && !t_object->checkToPlanet())
    {
        int closestGrappleIndex = -1;
        float lowestDist = 0.0f;


        for (int i = 0; i < GRAPPLE_AMOUNT; i++)
        {
            // Check which is the closest grapple
            float dist = pointToPointDist(grapples[i].getStartPoint(), t_object->getPos());

            if (!grapples[i].isActive())
            {
                if (dist > lowestDist) // > becasuse it works idk
                {
                    lowestDist = dist;
                    closestGrappleIndex = i;
                }
            }
        }
        if (closestGrappleIndex >= 0)
        {
            if (grapples[closestGrappleIndex].shoot(t_object)) // Returns true if an object was grabbed
            {
                objectsGrabbed++;
                maxGrappledObjects++;
            }
        }
    }
}

void Player::releaseGrapple(Vector2 t_releaseDir, bool t_toPlanet)
{
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].release(t_releaseDir, t_toPlanet);
        maxGrappledObjects = 0;
    }
}

void Player::setGrapplePos()
{
    int currentGrapple = 0;
    // Top grapples
    for (float angle = 45; angle <= 135; angle += 30.0f)
    {
        Vector2 pointOnCir;
        // Convert degrees to radians
        float radians = (rotation + angle) * (PI / 180.0f);

        // Get evenly spaced point on the circle
        pointOnCir.x = position.x + ((RADIUS - 10) * cos(radians));
        pointOnCir.y = position.y + ((RADIUS - 10) * sin(radians));

        // Assign to the corresponding grapple slot
        grapples[currentGrapple].setStartPos(pointOnCir, this->position);
        currentGrapple++;
    }
    // Bottom
    for (float angle = 236; angle < 315; angle += 22.5f)
    {
        Vector2 pointOnCir;
        // Convert degrees to radians
        float radians = (rotation + angle) * (PI / 180.0f);

        // Get evenly spaced point on the circle
        pointOnCir.x = position.x + ((RADIUS - 10) * cos(radians));
        pointOnCir.y = position.y + ((RADIUS - 10) * sin(radians));

        // Assign to the corresponding grapple slot
        grapples[currentGrapple].setStartPos(pointOnCir, this->position);
        currentGrapple++;
    }
}

void Player::setGrappleAgression()
{
    int amountOfActiveGrapples = 0;
    // Find the amount that are active
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        if (grapples[i].isActive())
        {
            amountOfActiveGrapples++;
        }
    }

    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        if (!grapples[i].isActive())
        {
            grapples[i].setAggressionLevel(amountOfActiveGrapples);
        }
    }
}

void Player::update(Vector2 t_leftStickDir, Vector2 t_cursorPos)
{   
    if (alive)
    {
        updateModels();

        if (IsGamepadAvailable(0))
        {
            controllerMovement(t_leftStickDir, t_cursorPos);
        }
        else
        {
            move();
        }
    
        if (invincible)
        {
            invincibleClock();
        }
    
        // Grapple updates
        setGrapplePos();
        setGrappleAgression();
        for (int i = 0; i < GRAPPLE_AMOUNT; i++)
        {
            grapples[i].update();
        }

        position3D = convertToMiddleCoords(position);
    }

    particles.update();
}

void Player::updateModels()
{
    Matrix tiltMatrix = MatrixRotateX(DEG2RAD * tilt);
    Matrix spinMatrix = MatrixRotateY(DEG2RAD * -rotation);

    // Combine the rotations
    Matrix rotationMatrix = MatrixMultiply(spinMatrix, tiltMatrix);

    // Apply the combined rotation to the planet models
    ring.transform = rotationMatrix;
    beams.transform = rotationMatrix;
    hull.transform = rotationMatrix;
}

void Player::kill()
{
    alive = false;

    particles.setValues(position, 360, 0);
    particles.spawn(10);

    grapples->release({0, 0}, false);

    PlaySound(deathSound);
}

void Player::invincibleClock()
{
    if (invincibilityTimer < INVINCIBILITY_DURATION)
    {
        invincibilityTimer += GetFrameTime();

        color = RED;
    }
    else
    {
        invincibilityTimer = 0;
        invincible = false;
        color = WHITE;
    }
}

void Player::heal(int t_amount)
{
    health += t_amount;

    if (health > MAX_HEALTH)
    {
        health = MAX_HEALTH;
    }
}

void Player::takeDamage(int t_amount)
{
    if (!invincible && alive)
    {
        if (health > t_amount)
        {
            health -= t_amount;
            invincible = true;

            SetSoundPitch(damageSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
            PlaySound(damageSound);

            particles.setValues(position, 360, 0);
            particles.spawn();

            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 15);
        }
        else if (health == t_amount && !lastHit)
        {
            health = 1;
            lastHit = true;
            invincible = true;

            SetSoundPitch(damageSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
            PlaySound(damageSound);

            particles.setValues(position, 360, 0);
            particles.spawn();

            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 15);
        }
        else
        {
            kill();
            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE + 5, 15);
        }
    }
}

void Player::initHealthBar()
{
    barPos = {SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.9f};
    barLength = SCREEN_WIDTH / 2.0f;
}

void Player::drawHealthBar()
{
    if (barOn)
    {
        DrawRectangle(barPos.x, barPos.y, barLength, barHeight, BLACK);

        DrawRectangle(barPos.x, barPos.y, (health * barLength) / 100, barHeight, YELLOW);
    }
}

void Player::dropEverything()
{
    releaseGrapple({0, 0}, false);
}

void Player::reset()
{
    dropEverything();

    health = MAX_HEALTH;
}

bool Player::respawn()
{
    if (!alive)
    {
        if (respawnTimer < RESPAWN_DUR)
        {
            respawnTimer += GetFrameTime();
        }
        else
        {
            respawnTimer = 0.0f;
            position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    
            alive = true;
            health = MAX_HEALTH;
            lastHit = false;

            stage = 0;

            Transition::begin();
    
            return true;
        }
    }

    return false;
}

Vector3 Player::convertToMiddleCoords(Vector2 t_originalCoords)
{
    float normalizedX = normalizeSigned(t_originalCoords.x, 0.0f, SCREEN_WIDTH);
    float normalizedY = normalizeSigned(t_originalCoords.y, 0.0f, SCREEN_HEIGHT);
    
    return {normalizedX * SCREEN_BOUNDS_X, -normalizedY * SCREEN_BOUNDS_Y, MIDDLEGROUND_POS.z};
}

bool Player::checkIfGrabbing()
{
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        if (grapples[i].checkIfGrabbing())
        {
            return true;
        }
    }

    return false;
}