#include "../include/Button.h"
#include "../include/reasings.h"
#include "../include/Planet.h"

Button::Button(Sound &t_breakSound, Sound &t_grabbedSound, Texture2D &t_texture, Vector2 t_pos, int t_size, void (*t_effect)())
    : Object(t_breakSound, t_grabbedSound, t_texture, t_pos, t_size), effect(t_effect)
{
    originalPos = t_pos;

    originalScale = 0.5f;
	scale = originalScale;

	velocity = {0.0f, 0.0f};
}

void Button::update()
{
    if (!checkGrabbed() && checkIfPickedUp())
    {
        if (!checkToPlanet())
        {
            if (elapsedTimeBounce < TIME_TAKEN_TO_POS)
            {
                elapsedTimeBounce += GetFrameTime();
            }

            position.x = EaseElasticOut(elapsedTimeBounce, startPos.x, originalPos.x - startPos.x, TIME_TAKEN_TO_POS);
            position.y = EaseElasticOut(elapsedTimeBounce, startPos.y, originalPos.y - startPos.y, TIME_TAKEN_TO_POS);
        }

        color = baseColor;
    }

	particleSpawner.update();
}

void Button::draw()
{
    if (active)
	{
		DrawTextureEx(texture, {position.x - radius, position.y - radius}, 0, scale, color);
	}
	particleSpawner.draw();
}

void Button::released(Vector2 t_releaseDir, bool t_toPlanet)
{
    elapsedTimeBounce = 0.0f;
    startPos = position;

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

void Button::movementToPlanet(Planet &t_planet)
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

            if (canDamage)
            {
                t_planet.takeDmg(damage); 
            }
	
			active = false;
			toPlanet = false;
		}
	
		if (scale <= 0)
		{
			active = false;
		}
	}
}

void Button::reset()
{
	active = true;
	position = originalPos;
	scale = originalScale;

	toPlanet = false;
}

