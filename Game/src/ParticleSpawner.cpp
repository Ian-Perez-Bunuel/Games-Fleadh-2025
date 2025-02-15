#include "../include/ParticleSpawner.h"

void ParticleSpawner::setValues(Vector2 t_pos, int t_angleLeeway, float t_dirAngle)
{
    position = t_pos;
    angleLeeway = t_angleLeeway;
    dirAngle = t_dirAngle;
}

void ParticleSpawner::addColor(Color t_color)
{
    colors.push_back(t_color);
}

void ParticleSpawner::spawn()
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        float randDir = randomizeDir();
        int randSpeed = (rand() % 5) + 5;
        int randSize = (rand() % 7) + 7;
        Color randColor = randomizeColor();

        particles.push_back(Particle(position, randSize, randSpeed, randDir, randColor));
    }

    active = true;
}

bool ParticleSpawner::checkIfParticalsActive()
{
    for (Particle& p : particles)
    {
        if (p.isActive())
        {
            return false; // At least one particle  is active
        }
    }

    // No particles are active
    return true;
}

void ParticleSpawner::update()
{
    if (checkIfParticalsActive())
    {
        particles.clear();
        active = false;
    }
    
    if (active)
    {
        for (Particle& p : particles)
        {
            if (p.isActive())
            {
                p.move();
            }
        }
    }
}

void ParticleSpawner::draw()
{
    for (Particle& p : particles)
    {
        if (p.isActive())
        {
            p.draw();
        }
    }
}



float ParticleSpawner::randomizeDir()
{
    // Randomize direction
    float result = 0.0f;
    float randLeewat = rand() % angleLeeway;
    if (rand() % 2 == 0)
    {
        result = dirAngle + randLeewat;
    }
    else
    {
        result = dirAngle - randLeewat;
    }

    return result;
}

Color ParticleSpawner::randomizeColor()
{
    return colors[rand() % colors.size()];;
}