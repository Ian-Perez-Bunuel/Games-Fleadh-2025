#pragma once

#include "raylib.h"
#include "stdio.h"
#include "../include/Object.h"

class Button : public Object
{
public:
    Button(Sound& t_breakSound, Sound& t_grabbedSound, Texture2D& t_texture, Vector2 t_pos = {0.0f, 0.0f}, int t_size = 10);


    void update() override;
    void draw() override;

private:
    Vector2 originalPos;
};