#include "../include/Player.h"
#include "../include/Globals.h"
Player::Player()
{
    position = {100, 100};
}

void Player::initialize()
{
    texture = LoadTexture("resources/Art/player.png");
}

void Player::move()
{
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

    // Player can now travel from one side to the other using the borders
    screenWrapping();

    position.x += velocity.x;
    position.y += velocity.y;
}

void Player::controllerMovement(Vector2 t_stickDir)
{
    velocity.x += t_stickDir.x;
    velocity.y += t_stickDir.y;

    if (t_stickDir.x == 0 && t_stickDir.y == 0)
    {
        velocity.x *= FRICTION;
        velocity.y *= FRICTION;
    }

    // Make sure the player doesnt go too fast
    capSpeed();

    // Player can now travel from one side to the other using the borders
    screenWrapping();

    position.x += velocity.x;
    position.y += velocity.y;
}

void Player::screenWrapping()
{   
    // X coords wrapping
    if (position.x < -RADIUS)
    {
        position.x = SCREEN_WIDTH + RADIUS;
    }
    else if (position.x > SCREEN_WIDTH + RADIUS)
    {
        position.x = -RADIUS;
    }
    // X coords wrapping
    if (position.y < -RADIUS)
    {
        position.y = SCREEN_HEIGHT + RADIUS;
    }
    else if (position.y > SCREEN_HEIGHT + RADIUS)
    {
        position.y = -RADIUS;
    }
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

void Player::draw()
{
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].draw(); 
    }

    //DrawCircleLinesV(position, RADIUS, YELLOW);
    DrawTextureEx(texture, {position.x - RADIUS, position.y - RADIUS}, 0, 0.075f, WHITE);
}

void Player::shootGrapple(std::shared_ptr<Object> t_target)
{
    int closestGrappleIndex = -1;
    float lowestDist = 0.0f;

    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        // Check which is the closest
        float dist = pointToPointDist(grapples[i].getStartPoint(), t_target->getPos());

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
        grapples[closestGrappleIndex].shoot(t_target);
    }
}

void Player::releaseGrapple(Vector2 t_releaseDir)
{
    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].release(t_releaseDir);
    }
}

void Player::update(Vector2 t_leftStickDir)
{   
    controllerMovement(t_leftStickDir);
    move();

    int currentGrapple = 0;
    // Top grapples
    for (float angle = 45; angle <= 135; angle += 30.0f)
    {
        Vector2 pointOnCir;
        // Convert degrees to radians
        float radians = angle * (PI / 180.0f);

        // Get evenly spaced point on the circle
        pointOnCir.x = position.x + (RADIUS * cos(radians));
        pointOnCir.y = position.y + (RADIUS * sin(radians));

        // Assign to the corresponding grapple slot
        grapples[currentGrapple].setStartPos(pointOnCir, this->position);
        currentGrapple++;
    }
    // Bottom
    for (float angle = 236; angle < 315; angle += 22.5f)
    {
        Vector2 pointOnCir;
        // Convert degrees to radians
        float radians = angle * (PI / 180.0f);

        // Get evenly spaced point on the circle
        pointOnCir.x = position.x + (RADIUS * cos(radians));
        pointOnCir.y = position.y + (RADIUS * sin(radians));

        // Assign to the corresponding grapple slot
        grapples[currentGrapple].setStartPos(pointOnCir, this->position);
        currentGrapple++;
    }
    


    for (int i = 0; i < GRAPPLE_AMOUNT; i++)
    {
        grapples[i].update();
    }
}
