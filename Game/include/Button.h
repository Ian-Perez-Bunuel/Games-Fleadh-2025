#pragma once

#include "raylib.h"
#include "stdio.h"
#include "../include/Object.h"

class Button : public Object
{
public:
    Button(Sound& t_breakSound, Sound& t_grabbedSound, Texture2D& t_texture, Vector2 t_pos = {0.0f, 0.0f}, int t_size = 10, void (*t_effect)() = nullptr);

    void setBaseColor(Color t_color) { baseColor = t_color; }

    void update() override;
    void draw() override;
    void released(Vector2 t_releaseDir, bool t_toPlanet) override;

    void movementToPlanet(Planet& t_planet) override;

    void setCanDamage() { canDamage = true; }

    void (*effect)(); // Define a pointer funtions. This will allow it to copy another function's definition

private:
    Vector2 startPos;
    Vector2 originalPos;

    bool canDamage = false;

    const int TIME_TAKEN_TO_POS = 2;
    float elapsedTimeBounce = 0.0f;

    Color baseColor;
};