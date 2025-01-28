#include "../include/Object.h"

Object::Object(Vector2 t_pos) : position(t_pos)
{
}

void Object::update()
{
	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();
}

void Object::draw()
{
    DrawCircleV(position, RADIUS, RED);
}

void Object::dragging(Vector2 t_anchorPos)
{
	// Variables
	float lenght = 0.0f;
	Vector2 heading = {0.0f, 0.0f};


	heading.x = (t_anchorPos.x) - position.x;
	heading.y = (t_anchorPos.y) - position.y;
	lenght = sqrtf((heading.x * heading.x) + (heading.y * heading.y)); // find the distance

	heading.x = heading.x / lenght;
	heading.y = heading.y / lenght;
	heading.x = heading.x * (lenght * 1.5f); // change speed to the actual speed
	heading.y = heading.y * (lenght * 1.5f); // change speed to the actual speed
	velocity = heading;

	// velocity.x *= speed / lenght;
	// velocity.y *= speed / lenght;

	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();
	// Rigid dist restriction
    // position = scaleVectorLenght(t_anchorPos, position, t_dist);
}

void Object::released()
{
	velocity.x *= 2.5f;
	velocity.y *= 2.5f;
}

Vector2 Object::scaleVectorLenght(Vector2 t_startPoint, Vector2 t_endPoint, int t_distance)
{
	// Calculate the total length of the line
	float totalLength = vectorLenght(t_startPoint, t_endPoint);

	// Calculate the ratio of the desired length to the total length of the line
	float ratio = t_distance / totalLength;

	// Calculate the coordinates of the point at the desired length along the line
	float newX = t_startPoint.x + ratio * (t_endPoint.x - t_startPoint.x);
	float newY = t_startPoint.y + ratio * (t_endPoint.y - t_startPoint.y);

	return { newX, newY };
}
