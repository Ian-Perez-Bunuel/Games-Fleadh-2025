#include "../include/Projectile.h"
#include <cmath>
#include <random>
#include "raymath.h"
#include "../include/DifficultyManager.h"

#include "../include/Globals.h"

Projectile::Projectile(Model& t_model, Color t_color, Vector3 t_pos, Vector3 t_targetPos, Player& t_player)
    : model(t_model), position(t_pos), targetPos(t_targetPos), player(t_player), color(t_color)
{
    active = true;

    particleSpawner.addColor(color);

    explosionSound = LoadSound("resources/Sound/rocketExplosion.wav");
    SetSoundVolume(explosionSound, 0.2f);
}

void Projectile::draw()
{
    if (active)
    {
        DrawModel(model, position, 0.1f, color);
        DrawModelWires(model, position, 0.1f, BLACK);
    }
}

void Projectile::drawParticles()
{
    if (explode || !particleSpawner.checkIfParticalsActive())
    {
        // DrawCircleV(convertToGameCoords(position), explosionRadius, RED);
        particleSpawner.draw();
    }
}

void Projectile::update()
{
    if (active)
    {
        moveToTarget();
    }
}

void Projectile::moveToTarget()
{
    rotateToTarget();

    // Compute the direction vector (pointing toward the target)
    Vector3 direction = {
        targetPos.x - position.x,
        targetPos.y - position.y,
        targetPos.z - position.z
    };

    // Calculate the length (magnitude) of the direction vector
    float length = sqrtf(direction.x * direction.x + 
                         direction.y * direction.y + 
                         direction.z * direction.z);

    // Normalize the direction vector
    if (length > 0.0001f) 
    {
        direction.x /= length;
        direction.y /= length;
        direction.z /= length;
    }   

    // Move projectile
    position.x += direction.x * DifficultyManager::getOrdinanceSpeed();
    position.y += direction.y * DifficultyManager::getOrdinanceSpeed();
    position.z += direction.z * DifficultyManager::getOrdinanceSpeed();
    
    // Rotating the model to point at its target pot
    // Cross product for rotation. (Used for getting forward)

    if (position.z >= targetPos.z)
    {
        active = false;
        explode = true;
    }
    else if (direction.x == 0 && direction.y == 0 && direction.z == 0)
    {
        active = false;
        explode = true;
    }
}

void Projectile::rotateToTarget()
{
    Matrix tiltMatrix = MatrixRotateX(DEG2RAD * -90);
    Matrix spinMatrix = MatrixRotateY(DEG2RAD * angleBetweenAtan2(position, targetPos));

    // Combine the rotations: spin is applied after tilt
    Matrix rotationMatrix = MatrixMultiply(spinMatrix, tiltMatrix);

    // Apply the combined rotation to the planet model
    model.transform = rotationMatrix;
}

void Projectile::explosion()
{
    if (explode)
    {
        if (explosionTimer < EXPLOSION_DURATION)
        {
            Vector2 position2D = convertToGameCoords(position);

            if (!explosionPlayed)
            {
                SetSoundPitch(explosionSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
                PlaySound(explosionSound);

                explosionPlayed = true;
            }

            particleSpawner.setValues(position2D, 360, 0);
            particleSpawner.spawn();


            float dist = Vector2Distance(position2D, player.getPos());
            if (dist <= player.getRadius() + explosionRadius)
            {
                player.takeDamage(25);
            }

            explosionTimer += GetFrameTime();
        }
        else
        {
            explosionTimer =  0.0f;
            explode = false;
        }
    }

    if (!particleSpawner.checkIfParticalsActive())
    {
        particleSpawner.update();
    }
}



Vector2 Projectile::convertToGameCoords(Vector3 t_originalCoords)
{
    float normalizedX = Normalize(t_originalCoords.x, -SCREEN_BOUNDS_X, SCREEN_BOUNDS_X);
    float normalizedY = Normalize(t_originalCoords.y, -SCREEN_BOUNDS_Y, SCREEN_BOUNDS_Y);
    
    return {normalizedX * SCREEN_WIDTH, -(normalizedY * SCREEN_HEIGHT) + SCREEN_HEIGHT};
}

float Projectile::angleBetweenAtan2(Vector3 t_a, Vector3 t_b)
{
    Vector3 crossProd = cross(t_a, t_b);
    double crossMag = magnitude(crossProd);
    double dotProd = dot(t_a, t_b);
    
    // atan2 returns the angle in radians
    return std::atan2(crossMag, dotProd);
}