#include "../include/Object.h"

Object::Object(Vector2 t_pos) : position(t_pos)
{
}

void Object::update()
{
    // position.x += velocity.x;
    // position.y += velocity.y;

    // velocity.x *= 0.9;
    // velocity.y *= 0.9;
}

void Object::draw()
{
    DrawCircleV(position, RADIUS, RED);
}

void Object::restrainDist(float t_dist, Vector2 t_anchorPos)
{
    position = scaleVectorLenght(t_anchorPos, position, t_dist);
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
