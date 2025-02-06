#include "../include/Object.h"
#include <random>


Object::Object(Vector2 t_pos) : position(t_pos)
{
	texture = LoadTexture("resources/Art/object.png");
	radius = (rand() % 30) + 10;
	mass = radius * 2;
	speed -= mass;
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
	DrawTextureEx(texture, {position.x - radius, position.y - radius}, 0, radius / 400.0f, WHITE);
}

void Object::grab(Vector2 t_anchorPos)
{
	if (!grabbed)
	{
		angle = radiansToDegrees(atan2(position.y - t_anchorPos.y, position.x - t_anchorPos.x));
		grabbed = true;
	}
}

void Object::held(Vector2 t_anchorPos, float t_dist)
{
	if (grabbed)
	{
		angle += speed * GetFrameTime();
		if (anchorDist < 0.0f)
		{
			anchorDist = t_dist;
		}	

		// Restrict angle
		if (angle >= 360)
		{
			angle = 0;

			if (speed < MAX_SPEED)
			{
				speed *= 1.25;
			}
		}

		// Get the distance from the anchor to the object
		// float distance = sqrt(pow((t_anchorPos.x - position.x), 2) + pow((t_anchorPos.y - position.y), 2));

		// Rigid dist restriction
		position.x = t_anchorPos.x + anchorDist * cos(degreesToRadians(angle));
		position.y = t_anchorPos.y + anchorDist * sin(degreesToRadians(angle));
	}
}


void Object::released(Vector2 t_releaseDir)
{
	grabbed = false;

	velocity.x = t_releaseDir.x - position.x;
	velocity.y = t_releaseDir.y - position.y;

	velocity.x *= 0.05 * (speed / MAX_SPEED);
	velocity.y *= 0.05 * (speed / MAX_SPEED);
}
