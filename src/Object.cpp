#include "../include/Object.h"
#include <random>


Object::Object(Vector2 t_pos) : position(t_pos)
{
	texture = LoadTexture("resources/Art/object.png");
	radius = (rand() % 30) + 10;
	mass = radius * 2;
	speed -= mass;

	velocity = {2, 0};

	color = WHITE;
}

void Object::update()
{
	if (!grabbed)
	{
		position.x += velocity.x;
		position.y += velocity.y;
	}
}

void Object::draw()
{
    // DrawCircleV(position, radius, RED);
	DrawTextureEx(texture, {position.x - radius, position.y - radius}, 0, radius / 400.0f, color);
}

void Object::grab()
{
	if (!grabbed)
	{
		grabbed = true;
	}

	color = YELLOW;
}

bool Object::moveToRotationArea(Vector2 t_anchorPos, float t_targetDist)
{
    float currentDist = pointToPointDist(t_anchorPos, position);
    
    if (fabs(currentDist - t_targetDist) < 3.0f) // Slight tolerance to avoid oscillation
    {
        return true; // Object is close enough to the target distance
    }

    Vector2 direction = { position.x - t_anchorPos.x, position.y - t_anchorPos.y };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.0f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    // Smooth movement towards the target distance using easing
    float moveSpeed = 10.0f * GetFrameTime(); // Lower speed for smoother transition
    float newDist = currentDist + (t_targetDist - currentDist) * (1 - exp(-moveSpeed));

    // Update position smoothly
    position.x = t_anchorPos.x + newDist * direction.x;
    position.y = t_anchorPos.y + newDist * direction.y;

    return false; // Still adjusting distance
}



void Object::held(Vector2 t_anchorPos, float t_dist)
{
    if (grabbed)
    {
        if (moveToRotationArea(t_anchorPos, t_dist)) 
		{
			float currentDist = pointToPointDist(t_anchorPos, position);
			position.x = t_anchorPos.x + t_dist * (position.x - t_anchorPos.x) / currentDist;
			position.y = t_anchorPos.y + t_dist * (position.y - t_anchorPos.y) / currentDist;
			correctDist = true;
		}
		

        if (correctDist)
        {
            if (anchorDist < 0.0f)
            {
                anchorDist = t_dist;

                // Compute initial speed from velocity magnitude
                float initialSpeed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
                speed = initialSpeed; // Start with object's velocity magnitude
            }

            // Smoothly interpolate speed from initial speed to MAX_SPEED
            float easingFactor = 0.2f; 
            speed += (MAX_SPEED - speed) * (1 - exp(-easingFactor * GetFrameTime()));

            angle += speed * GetFrameTime();

            checkForSpeedIncrease();

            // Gradual movement instead of instant snapping
            float lerpFactor = 1 - exp(-5.0f * GetFrameTime()); // Smooth transition
            position.x = (1 - lerpFactor) * position.x + lerpFactor * (t_anchorPos.x + t_dist * cos(degreesToRadians(angle)));
            position.y = (1 - lerpFactor) * position.y + lerpFactor * (t_anchorPos.y + t_dist * sin(degreesToRadians(angle)));
        }
		else
		{
			angle = radiansToDegrees(atan2(position.y - t_anchorPos.y, position.x - t_anchorPos.x));
		}
    }
}


void Object::checkForSpeedIncrease()
{
	// Restrict angle within (0, 360)
	if (angle >= 360.0f)
	{
		angle -= 360.0f;

		// Slightly increase speed upon full rotation
		if (speed < MAX_SPEED)
		{
			speed *= 1.05f; // Adjust acceleration factor as needed
		}
	}
}


void Object::released(Vector2 t_releaseDir)
{
	grabbed = false;

	
	velocity.x = t_releaseDir.x - position.x;
	velocity.y = t_releaseDir.y - position.y;
	
	velocity.x *= 0.05 * (speed / MAX_SPEED);
	velocity.y *= 0.05 * (speed / MAX_SPEED);
	
	speed = 0;

	correctDist = false;
}
