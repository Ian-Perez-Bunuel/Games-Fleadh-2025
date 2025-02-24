#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include "../include/Globals.h"
#include <random>
#include "rlgl.h"

void Planet::init(Vector3 t_pos, Color t_color)
{
    position = t_pos;
    model = LoadModel("resources/Art/3D/planet4.glb");

    core = LoadModel("resources/Art/3D/core.glb");
    shield = LoadModel("resources/Art/3D/shield.glb");

    color = t_color;
    genExplosionTexture();
    
    explosionShader = LoadShader("resources/Shaders/explosion.vs", "resources/Shaders/explosion.fs");

    displacementIntensityLocationInShader = GetShaderLocation(explosionShader, "displacementIntensity");
    explosionTimerLocationInShader = GetShaderLocation(explosionShader, "explosionTimer");

    SetShaderValue(explosionShader, displacementIntensityLocationInShader, &displacementIntensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(explosionShader, explosionTimerLocationInShader, &explosionTimer, SHADER_UNIFORM_FLOAT);

    // used to pass to the shader
    Vector4 normalizedColor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
    int colorLocationInShader = GetShaderLocation(explosionShader, "color");
    SetShaderValue(explosionShader, colorLocationInShader, &normalizedColor, SHADER_UNIFORM_VEC4);

    // Assign shader to the planet model
    model.materials[0].shader = explosionShader;
    core.materials[0].shader = explosionShader;

    deathParticles.addColor(color);
}

void Planet::updateRotation()
{
    pitch += 0.25f;  // Increment rotation over time
    
    // Create a rotation matrix for the tilt around the X axis,
    // then a spin rotation around the Y axis.
    Matrix tiltMatrix = MatrixRotateX(DEG2RAD * tilt);
    Matrix spinMatrix = MatrixRotateY(DEG2RAD * pitch);

    // Combine the rotations
    Matrix rotation = MatrixMultiply(spinMatrix, tiltMatrix);

    // Apply the combined rotation to the planet models
    model.transform = rotation;
    core.transform = rotation;
}

void Planet::update()
{
    if (explosionActive)
    {
        explosion();
    }
    if (defeated)
    {
        whileDead();
    }

    updateRotation();
}

void Planet::draw()
{
    rlSetLineWidth(0.1f);
    DrawModel(core, position, 0.5, color);
    DrawModelWires(core, position, 0.5f, color + coreTint);

    rlSetLineWidth(2.0f);
    if (!defeated)
    {
        DrawModelWires(model, position, 0.75f, color);
    }

    // rlSetLineWidth(10.0f);
    // DrawModel(shield, position, 1.0, WHITE);
    // DrawModelWires(shield, position, 1.0, BLUE);
}

void Planet::drawParticles()
{
    deathParticles.draw();
}

void Planet::whileDead()
{
    float dist = sqrtf(pow(deathPos.x - position.x, 2) + pow(deathPos.y - position.y, 2) + pow(deathPos.z - position.z, 2));
    // If not at Death pos move to it
    if (dist > 0.1f)
    {
        moveToDeathPos();
    }
    else
    {
        deathAnimation();
    }
}

void Planet::deathAnimation()
{
    if (particleTimer < PARTICLE_WAIT)
    {
        particleTimer += GetFrameTime();
    }
    else
    {
        particleTimer = 0.0f;

        deathParticles.setValues({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, 360, 0);

        if (invertedParticles)
        {
            deathParticles.invertedSpawn(5);
            invertedParticles = false;
        }
        else
        {
            deathParticles.spawn(5);
            invertedParticles = true;
        }
    }

    deathParticles.update();
}

void Planet::moveToDeathPos()
{
    // Compute the direction vector
    Vector3 direction = {
        deathPos.x - position.x,
        deathPos.y - position.y,
        deathPos.z - position.z
    };

    // Calculate the length of the direction vector
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

    // Move Core to deathPos
    position.x += direction.x * 0.1f;
    position.y += direction.y * 0.1f;
    position.z += direction.z * 0.1f;
}

void Planet::takeDmg(int t_damage)
{
    if (health > t_damage)
    {
        health -= t_damage;

        explosionActive = true;
        
        SceneCamera::screenShake(SceneCamera::MEDIUM_SHAKE, 10);
    }
    else
    {
        defeated = true;
    }  
}

void Planet::explosion()
{
    // Update explosion effect
    if (maxDistHit)
    {
        if (explosionTimer >= 0.0f)
        {
            explosionTimer -= GetFrameTime() * 2;
        }
        else
        {
            maxDistHit = false;
            explosionActive = false;
        }
    }
    else
    {
        if (explosionTimer < 1.0f)
        {
            explosionTimer += GetFrameTime() * 4;
        }
        else if (maxDistHit == false)
        {
            maxDistHit = true;
        }
    }

    displacementIntensity = explosionTimer;

    SetShaderValue(explosionShader, displacementIntensityLocationInShader, &displacementIntensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(explosionShader, explosionTimerLocationInShader, &explosionTimer, SHADER_UNIFORM_FLOAT);
}

void Planet::genExplosionTexture()
{
    // Generate Perlin Noise and Gradiant texture
    int explosion_image_width = 1024;
    int explosion_image_height = 1024;

    Image perlinNoiseImage = GenImagePerlinNoise(explosion_image_width, explosion_image_height, 75, 75, 5.0f);
    Image radialGradientImage = GenImageGradientRadial(explosion_image_width, explosion_image_height, 0.0f, WHITE, BLACK);

    // Combine so its from center see image generated
    for (int y = 0; y < explosion_image_height; y++)
    {
        for (int x = 0; x < explosion_image_height; x++)
        {
            // Get the pixel colors from the Perlin and Gradient images
            Color perlinNoiseColor = GetImageColor(perlinNoiseImage, x, y);
            Color radialGradientColor = GetImageColor(radialGradientImage, x, y);

            // Blend the colors together
            // float factor = ((float)radialGradientColor.r * (float)radialGradientColor.g * (float)radialGradientColor.b) / 255.0f;
            float factor = ((float)radialGradientColor.g) / 255.0f;
            Color combinedColor = ColorLerp(perlinNoiseColor, radialGradientColor, factor);

            // Set the pixel color in the Perlin noise image to the blended color
            ImageDrawPixel(&perlinNoiseImage, x, y, combinedColor);
        }
    }

    // Now load the explosion_driver texture
    explosion_driver = LoadTextureFromImage(perlinNoiseImage);

    // Give details to shader
    perlinNoiseTextureLocationInShader = GetShaderLocation(explosionShader, "perlinNoiseTexture");
    SetShaderValueTexture(explosionShader, perlinNoiseTextureLocationInShader, explosion_driver);

    // Cleanup generated images, these are not needed now
    UnloadImage(perlinNoiseImage);
    UnloadImage(radialGradientImage);
}