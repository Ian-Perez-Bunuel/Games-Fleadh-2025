#include "../include/Object.h"
#include <random>
#include "../include/SceneCamera.h"
#include "../include/Globals.h"
#include "../include/ObjectManager.h"

#include "../include/Planet.h"


Object::Object(Sound& t_breakSound, Sound& t_grabbedSound, Texture2D& t_texture, Vector2 t_pos, int t_size, int dirAngle) 
	: texture(t_texture), position(t_pos), breakSound(t_breakSound), grabbedSound(t_grabbedSound), radius(t_size)
{
	mass = radius * 2;
	rotationSpeed -= mass;

	velocity = {cos(degreesToRadians(dirAngle)) * speed, sin(degreesToRadians(dirAngle)) * speed};

	active = true;

	color = BLUE;

	particleSpawner.addColor(color);

	// Change scale depending on size
	if (radius <= ObjectManager::SMALL) // Small size
	{
		scale = 0.34f;
	}
	else if (radius < ObjectManager::LARGE) // If size Medium
	{
		scale = 0.56f;
	}
	// Large is 1 so no change needed
}

void Object::update()
{
	if (!grabbed && active && !toPlanet)
	{
		move();

		invinsabilityCheck();

		loop();
	}

	if (velocity.x == 0.0f && velocity.y == 0.0f)
	{
		active = false;
	}

	particleSpawner.update();
}


void Object::draw()
{
	if (active)
	{
		// DrawCircleV(position, radius, RED);
		DrawTextureEx(texture, {position.x - radius, position.y - radius}, 0, scale, color);
	}
	particleSpawner.draw();
}

void Object::grab()
{
	if (!grabbed)
	{
		SetSoundPitch(grabbedSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
		PlaySound(grabbedSound);

		grabbed = true;
		beenPickedUp = true;
		color = YELLOW;

		particleSpawner.addColor(YELLOW);
	}

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
    float moverotationSpeed = 10.0f * GetFrameTime(); // Lower rotationSpeed for smoother transition
    float newDist = currentDist + (t_targetDist - currentDist) * (1 - exp(-moverotationSpeed));

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

                // Compute initial rotationSpeed from velocity magnitude
                float initialrotationSpeed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
                rotationSpeed = initialrotationSpeed; // Start with object's velocity magnitude
            }

            // Smoothly interpolate rotationSpeed from initial rotationSpeed to MAX_ROTATION_SPEED
            float easingFactor = 0.2f; 
            rotationSpeed += (MAX_ROTATION_SPEED - rotationSpeed) * (1 - exp(-easingFactor * GetFrameTime()));

            angle += rotationSpeed * GetFrameTime();

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

		// Slightly increase rotationSpeed upon full rotation
		if (rotationSpeed < MAX_ROTATION_SPEED)
		{
			rotationSpeed *= 1.05f; // Adjust acceleration factor as needed
		}
		else
		{
			rotationSpeed = MAX_ROTATION_SPEED;
			
			if (!hitMaxRotationSpeed)
			{
				hitMaxRotationSpeed = true;
				ObjectManager::increaseAmountMaxed();
			}	
		}
	}
}

void Object::loop()
{
	// Left Side
	if (position.x < -(SCREEN_WIDTH / 2))
	{
		active = false;
	}
	else if (position.x < -radius)
	{
		collidable = false;
	}
	// Right side
	else if (position.x > SCREEN_WIDTH + radius)
	{
		active = false;
	}

	// Top
	if (position.y < -(SCREEN_HEIGHT * 0.75f))
	{
		active = false;
	}
	else if (position.y < -radius)
	{
		collidable = false;
	}
	// Bottom
	else if (position.y > SCREEN_HEIGHT + radius)
	{
		active = false;
	}
}

bool Object::checkObjectCollisions(std::shared_ptr<Object> t_otherObject)
{
	if (!active || !t_otherObject->isActive() || toPlanet)
	{
		return false;
	}

	float dist = pointToPointDist(position, t_otherObject->getPos());
	if (dist < radius + t_otherObject->getRadius())
	{	
		SetSoundPitch(breakSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
		PlaySound(breakSound);
		return true;
	}

	return false;
}

void Object::destroy()
{
	active = false;

	particleSpawner.setValues(position, 360, 0);
	particleSpawner.spawn();

	SceneCamera::screenShake(SceneCamera::SMALL_SHAKE, 10);
}

void Object::invinsabilityCheck()
{
	if (invincibilityTimer < INVINCIBILITY_DURATION && !collidable)
	{
		invincibilityTimer++;
	}		
	else
	{
		collidable = true;

		invincibilityTimer = 0;
	}
}


void Object::released(Vector2 t_releaseDir, bool t_toPlanet)
{
	grabbed = false;
	toPlanet = t_toPlanet;

	if (toPlanet)
	{
		planetPos = t_releaseDir;
	}
	
	velocity.x = t_releaseDir.x - position.x;
	velocity.y = t_releaseDir.y - position.y;
	
	velocity.x *= 0.05 * (rotationSpeed / MAX_ROTATION_SPEED);
	velocity.y *= 0.05 * (rotationSpeed / MAX_ROTATION_SPEED);
	
	rotationSpeed = 0;

	correctDist = false;
}

void Object::move()
{
	position.x += velocity.x;
	position.y += velocity.y;

	if (beenPickedUp)
	{
		// Spawn trail particles
		if (sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)) >= 15)
		{
			Vector2 nextPos = {position.x + velocity.x, position.y + velocity.y};
			float oppositeAngle = atan2f(nextPos.y - position.y, nextPos.x - position.x) * RAD2DEG + 270;
			particleSpawner.setValues(position, 15, oppositeAngle);
			particleSpawner.spawn();
		}
	}
}

void Object::movementToPlanet(Planet& t_planet)
{
	if (active)
	{
		move();
		scale -= 0.005f;
	
		float leeway = 15.0f; // Adjust this value to your desired tolerance
		if (fabs(position.x - planetPos.x) < leeway && fabs(position.y - planetPos.y) < leeway)
		{
			destroy();
			
			int damage = (radius * (sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)))) / 40;
			t_planet.takeDmg(damage); 
	
			active = false;
			toPlanet = false;
		}
	
		if (scale <= 0)
		{
			active = false;
		}
	}
}