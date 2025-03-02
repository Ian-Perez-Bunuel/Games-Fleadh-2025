#include "../include/Button.h"


Button::Button(Sound &t_breakSound, Sound &t_grabbedSound, Texture2D &t_texture, Vector2 t_pos, int t_size)
    : Object(t_breakSound, t_grabbedSound, t_texture, t_pos, t_size)
{
    originalPos = t_pos;
}

void Button::update()
{
    if (beenPickedUp)
    {
        
    }

	particleSpawner.update();
}

void Button::draw()
{
    if (active)
	{
		// DrawCircleV(position, radius, RED);
		DrawTextureEx(texture, {position.x - radius, position.y - radius}, 0, scale, WHITE);
	}
	particleSpawner.draw();
}