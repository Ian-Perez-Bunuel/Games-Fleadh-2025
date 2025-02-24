#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <algorithm>

#include "../include/ParticleSpawner.h"

class Planet
{
public:
    void init(Vector3 t_pos = {0.0f, 0.0f, -9.0f}, Color t_color = WHITE);

    void update();
    void draw();
    void drawParticles();

    Vector3 getPos() { return position; }

    bool isDefeated() { return defeated; }
    void takeDmg(int t_damage);

private:    
    Model model;

    Model core;
    Model shield;

    
    Vector3 position;
    
    void updateRotation();
    float tilt = -45.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    
    void deathAnimation();
    void moveToDeathPos();
    void whileDead();
    static const int MAX_HEALTH = 1;
    int health = MAX_HEALTH;
    bool defeated = false;
    Vector3 deathPos = {0.0f, 0.0f, 1.0f};

    Color color;
    Color coreTint = {25, 25, 25, 255};

    void explosion();
    void genExplosionTexture();

    Shader explosionShader;
    // shader locations
    int displacementIntensityLocationInShader;
    int perlinNoiseTextureLocationInShader;
    int explosionTimerLocationInShader;

    Texture2D explosion_driver;
    float displacementIntensity = 0.0f;
    bool explosionActive = false;
    float explosionTimer = 0.0f;
    // Change this number for more dramatic effect
    bool maxDistHit = false;

    ParticleSpawner deathParticles;
    const float PARTICLE_WAIT = 1.0f;
    float particleTimer = 0.0f;
    bool invertedParticles = true;
};

// Overload the + operator for Raylib's Color type
inline Color operator+(const Color &lhs, const Color &rhs) 
{
    Color result;
    result.r = static_cast<unsigned char>(std::min(static_cast<int>(lhs.r) + static_cast<int>(rhs.r), 255));
    result.g = static_cast<unsigned char>(std::min(static_cast<int>(lhs.g) + static_cast<int>(rhs.g), 255));
    result.b = static_cast<unsigned char>(std::min(static_cast<int>(lhs.b) + static_cast<int>(rhs.b), 255));
    result.a = static_cast<unsigned char>(std::min(static_cast<int>(lhs.a) + static_cast<int>(rhs.a), 255));
    return result;
}