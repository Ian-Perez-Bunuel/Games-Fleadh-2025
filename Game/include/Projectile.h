#pragma once

#include "raylib.h"
#include "stdio.h"

#include "../include/ParticleSpawner.h"

class Projectile
{
public:
    Projectile(Vector3 t_pos, Vector3 t_targetPos);
    void draw();
    void update();
    void explosion(Vector3 t_playerPos);

    bool isExploding() { return explode; }
    bool isActive() { return active; }
    
private:
    Model model;

    void moveToTarget();

    bool active = false;
    bool explode = false;
    float explosionRadius = 0.2f;

    Vector3 position;
    float radius = 0.1f;
    float speed = 0.1f;

    Vector3 targetPos;
};