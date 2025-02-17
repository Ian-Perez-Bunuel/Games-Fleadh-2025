#pragma once

#include "raylib.h"
#include "stdio.h"

class Projectile
{
public:
    Projectile(Vector3 t_pos, Vector3 t_targetPos);
    void draw();

    void moveToTarget();

    bool isActive() { return active; }

private:
    bool active = false;

    Vector3 position;
    float radius = 0.1f;
    float speed = 0.1f;

    Vector3 targetPos;
};