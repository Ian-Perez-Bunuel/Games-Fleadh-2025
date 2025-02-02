#include "../include/Player.h"
#include "../include/Globals.h"

Player::Player()
{
    position = {100, 100};
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
        position.x = SCREEN_SIZE + RADIUS;
    }
    else if (position.x > SCREEN_SIZE + RADIUS)
    {
        position.x = -RADIUS;
    }
    // X coords wrapping
    if (position.y < -RADIUS)
    {
        position.y = SCREEN_SIZE + RADIUS;
    }
    else if (position.y > SCREEN_SIZE + RADIUS)
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
    DrawCircleV(position, RADIUS, BLUE);

    grapple.draw();
}

void Player::update(Vector2 t_leftStickDir)
{   
    controllerMovement(t_leftStickDir);
    move();

    if (grapple.isActive())
    {
        grapple.update();
    }
}
