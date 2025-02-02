#include "../include/Object.h"

Object::Object(Vector2 t_pos) : position(t_pos)
{
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
    DrawCircleV(position, RADIUS, RED);
}

void Object::grab(Vector2 t_anchorPos)
{
	if (!grabbed)
	{
		angle = atan2(position.y - t_anchorPos.y, position.x - t_anchorPos.x);
		grabbed = true;
	}
}

void Object::held(Vector2 t_anchorPos, float t_dist)
{
	angle += speed * GetFrameTime();
	if (grabbed)
	{
		// Rigid dist restriction
    	position.x = t_anchorPos.x + t_dist * cos(angle);
		position.y = t_anchorPos.y + t_dist * sin(angle);
	}

	// Aim line
	// Compute direction vector from anchor to object
        float dx = position.x - t_anchorPos.x;
        float dy = position.y - t_anchorPos.y;

        // Compute perpendicular direction (-dy, dx)
        Vector2 perpDir = { -dy, dx };

        // Normalize the perpendicular vector
        float length = sqrt(perpDir.x * perpDir.x + perpDir.y * perpDir.y);
        perpDir.x = (perpDir.x / length) * lineLength;
        perpDir.y = (perpDir.y / length) * lineLength;

        // Compute start and end points of the perpendicular line
        Vector2 lineStart = { position.x, position.y };
        Vector2 lineEnd = { position.x + perpDir.x, position.y + perpDir.y };

		velocity.x = perpDir.x / 100 * speed + 1;
		velocity.y = perpDir.y / 100 * speed + 1;

		DrawLineV(lineStart, lineEnd, RED);
}


void Object::released()
{
	grabbed = false;
	//velocity.x *= 2.5f;
	//velocity.y *= 2.5f;
}
