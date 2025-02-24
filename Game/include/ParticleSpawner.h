#pragma once

#include "../include/Particle.h"

class ParticleSpawner
{
public:
    void setValues(Vector2 t_pos, int t_angleLeeway, float t_dirAngle);

    void clearColors() { colors.clear(); }
    void addColor(Color t_color);

    void spawn();
    void spawn(int t_amount);
    void circularSpawn(int t_amount, float t_radius);

    void pause() { paused = !paused; }
    void update();
    void draw();

    bool checkIfParticalsActive();
private:
    // Randomizing functions
    float randomizeDir();
    Color randomizeColor();
    Vector2 posOnCircle(float t_angle, float t_radius);

    Vector2 position;

    const int MAX_PARTICLES = 10;
    std::vector<Particle> particles;

    int angleLeeway;
    float dirAngle;

    std::vector<Color> colors;

    bool paused = false;
    bool active = false;
};

