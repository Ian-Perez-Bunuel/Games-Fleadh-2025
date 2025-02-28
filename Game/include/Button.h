#pragma once

#include "raylib.h"
#include "stdio.h"

class Button
{
public:

	Button();

	void setup(Vector2 t_pos, float t_width, float t_height);
    float getHeight() { return height; }
    float getWidth() { return width; }

    void draw();

	bool checkForMouse();


private:

	float width;
	float height;

	Vector2 position;

	bool colliding = false;

};
