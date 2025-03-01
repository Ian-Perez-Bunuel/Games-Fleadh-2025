#include "../include/Player.h"
#include "../include/SceneCamera.h"
#include "../include/AchievementManager.h"
#include "../include/Globals.h"

Player::Player()
{
}

void Player::initialize()
{
    texture = LoadTexture("resources/Art/2D/player.png");
    position = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].init();
    }

    // Achievements
    AchievementManager::addGoalToAchievement("GRAB, GRAB, GRAB!!!", &objectsGrabbed, 1);
}

void Player::rotateToMouse()
{
    rotation = atan2f(GetMousePosition().y  - position.y, GetMousePosition().x  - position.x) * RAD2DEG;
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

void Player::move()
{
    rotateToMouse();
    boundryChecking();

    // key inputs
    if (IsKeyDown(KEY_W))
    {
        velocity.y -= SPEED;
    }
    else if (IsKeyDown(KEY_S))
    {
        velocity.y += SPEED;
    } 
    if (IsKeyDown(KEY_A))
    {
        velocity.x -= SPEED;
    }
    else if (IsKeyDown(KEY_D))
    {
        velocity.x += SPEED;
    }

    // If no movement keys pressed
    if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_A) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D))
    {
        velocity.x *= FRICTION;
        velocity.y *= FRICTION;
    }
    
    // Make sure the player doesnt go too fast
    capSpeed();


    position.x += velocity.x;
    position.y += velocity.y;
}

void Player::controllerMovement(Vector2 t_stickDir)
{
    boundryChecking();

    velocity.x += t_stickDir.x * 0.4f;
    velocity.y -= t_stickDir.y * 0.4f;

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
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
        Rectangle sourceRec = { 0, 0, (float)texture.width, (float)texture.height };
        Rectangle destRec = { position.x, position.y, texture.width * scale, texture.height * scale };
        Vector2 origin = { texture.width / 2.0f, texture.height / 2.0f };

        DrawTexturePro(texture, sourceRec, destRec, origin, rotation, color);
    }
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
            grapples[closestGrappleIndex].shoot(t_object, objectsGrabbed);
        }
    }
}

void Player::releaseGrapple(Vector2 t_releaseDir, bool t_toPlanet)
{
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].release(t_releaseDir, t_toPlanet);
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

void Player::update(Vector2 t_leftStickDir)
{   
    if (alive)
    {
        controllerMovement(t_leftStickDir);
        move();
    
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
    }
}

void Player::kill()
{
    alive = false;
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
            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 15);
        }
        else if (health == t_amount && !lastHit)
        {
            health = 1;
            lastHit = true;
            invincible = true;

            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 15);
        }
        else
        {
            kill();
        }
    }
}
