#include "../include/Particle.h"


Particle::Particle(Vector2 t_pos, float t_size, float t_speed, float t_dirAngle, Color t_color) : 
	position(t_pos), size(t_size), speed(t_speed), dirAngle(t_dirAngle), color(t_color)
{
    active = true;
}

void Particle::move()
{
	// 0 is Right, adding to 0 will make it go anti-clockwise
	float angleR = (dirAngle) * PI / 180;

	velocity.x = (sin(angleR) * speed);
	velocity.y = (cos(angleR) * speed) * -1;

	position.x += velocity.x;
    position.y += velocity.y;


	// Decrease size as it moves
	if (size > MIN_SIZE)
	{
		size -= 0.2f;
	}
	else
	{
		active = false;
	}
	// Decease Speed as it moves
	if (speed > MIN_SPEED)
	{
		speed -= 0.2f;
	}
	else
	{
		if (size > MIN_SIZE)
		{
			size -= 0.75f;
		}
		else
		{
			active = false;
		}
	}
}

void Particle::draw()
{
	DrawRectangleV(position, {size, size}, color);
}