// IAN PEREZ BUNUEL

#include "../include/Button.h"


Button::Button()
{
}

void Button::setup(Vector2 t_pos, float t_width, float t_height)
{
	// Get the dimensions
	width = t_width;
	height = t_height;

	position = t_pos;
}

bool Button::checkForMouse()
{
	// Check if the mouse is within the button on the x-axis
	if (GetMousePosition().x >= position.x - ((width) / 2) && GetMousePosition().x <= position.x + ((width) / 2))
	{
		// Check if the mouse is within the button on the y-axis
		if (GetMousePosition().y <= position.y + (height / 2) && GetMousePosition().y >= position.y - (height / 2))
		{
			// if colliding
			colliding = true;
			return colliding; 
		}
	}

	// if not colliding 
	colliding = false;

	return colliding; // if not colliding
}

void Button::draw()
{
    DrawRectangleLines(position.x - width / 2, position.y - height / 2, width, height, RED);
}