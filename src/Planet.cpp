#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include "../include/Globals.h"
#include "../include/DifficultyManager.h"
#include "../include/Player.h"
#include "../include/PlanetManager.h"
#include <random>
#include "rlgl.h"

void Planet::init(Vector3 t_pos, int t_maxHealth, Color t_color)
{
    projectiles.clear();
    deathParticles.clearColors();

    setOriginalPos(t_pos);
    position = t_pos;

    maxHealth = t_maxHealth;
    health = maxHealth;

    defeated = false;
    coreConsumed = false;

    //  Load each planet type
    planet1 = LoadModel("resources/Art/3D/planet1.obj");
    planet2 = LoadModel("resources/Art/3D/planet2.obj");
    planet3 = LoadModel("resources/Art/3D/planet3.obj");
    planet4 = LoadModel("resources/Art/3D/planet4.obj");

    destructionSound = LoadSound("resources/Sound/planetExplosion.wav");
    SetSoundVolume(destructionSound, 0.2f);
    coreCollectingSound = LoadSound("resources/Sound/coreCollect.wav");
    SetSoundVolume(coreCollectingSound, 0.2f);
    damageSound = LoadSound("resources/Sound/asteroidBreak.wav");
    SetSoundVolume(damageSound, 0.3f);
    
    // Randomize Planet model
    int randNum = rand() % 4;
    switch (randNum)
    {
    case 0:
        model = planet1;
        break;
    case 1:
        model = planet2;
        break;
    case 2:
        model = planet3;
        break;
    case 3:
        model = planet4;
        break;
    }

    // Load other models
    core = LoadModel("resources/Art/3D/core.glb");
    shield = LoadModel("resources/Art/3D/shield.obj");

    projectileModel = LoadModel("resources/Art/3D/missile.glb");

    color = t_color;
    // genExplosionTexture();
    
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
    TraceLog(LOG_INFO, "Assign shader to model materials");
    for (int i = 0; i < model.materialCount; i++)
    {
        model.materials[i].shader = explosionShader;
    }
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

void Planet::update(Vector3 t_playerPos3D, Player& t_player)
{
    if (defeated)
    {
        whileDead(t_playerPos3D, t_player.getPos());
    }
    else
    {
        if (explosionActive)
        {
            explosion();
        }

        shotClock(t_playerPos3D, t_player);

        for (Projectile& projectile : projectiles)
        {
            if (projectile.isActive())
            {
                projectile.update();
            }
            else
            {
                projectile.explosion();
            }
        }
        
        updateRotation();
    }

}

void Planet::draw()
{
    if (!defeated)
    {
        rlSetLineWidth(2.0f);
        DrawModelWires(model, position, 1.1f, color);

        if (projectiles.size() > 0)
        {
            for (Projectile projectile : projectiles)
            {
                projectile.draw();
            }
        }
    }
    if (!coreConsumed)
    {
        rlSetLineWidth(0.1f);
        DrawModel(core, position, 0.5, color);
        DrawModelWires(core, position, 0.5f, color + coreTint);
    }
}

void Planet::drawParticles()
{
    deathParticles.draw();
    if (!defeated)
    {
        if (projectiles.size() > 0)
        {
            for (Projectile projectile : projectiles)
            {
                projectile.drawParticles();
            }
        }
    }
}

void Planet::whileDead(Vector3 t_playerPos3D, Vector2 t_playerPos2D)
{
    float dist = sqrtf(pow(t_playerPos3D.x - position.x, 2) + pow(t_playerPos3D.y - position.y, 2) + pow(t_playerPos3D.z - position.z, 2));
    // If not at player position move to it
    if (dist > 0.1f && !coreConsumed)
    {
        moveToPos(t_playerPos3D);
    }
    else
    {
        if (!coreStart)
        {
            PlaySound(coreCollectingSound);
            coreStart = true;
        }
        // Anything in this if  statement will only be called once when the core is consumed
        if (!coreConsumed)
        {
            coreConsumed = true;

            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 120);
        }
        deathAnimation(t_playerPos2D);
    }
}


void Planet::deathAnimation(Vector2 t_playerPos)
{
    deathParticles.setValues(t_playerPos, 360, 0);
    // Increasing radius to max radius
    if (particleRadius <= MAX_PARTICAL_RADIUS && !maxRadiusHit)
    {
        deathParticles.circularSpawn(1, particleRadius);
        particleRadius += 6.0f;
    }
    else
    {
        maxRadiusHit = true;
    }
    // Decreasing radius towards the player
    if (particleRadius >= 30 && maxRadiusHit)
    {
        deathParticles.circularSpawn(1, particleRadius);
        particleRadius -= 1.5f;
    }
    else if (particleRadius != -1 && maxRadiusHit)
    {
        deathParticles.setValues(t_playerPos, 360, 0);
        deathParticles.spawn(10);

        particleRadius = -1; // Make sure that the explosion only happens once.
    }

    deathParticles.update();
}

void Planet::moveToPos(Vector3 t_targetPos)
{
    // Compute the direction vector
    Vector3 direction = {
        t_targetPos.x - position.x,
        t_targetPos.y - position.y,
        t_targetPos.z - position.z
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
    position.x += direction.x * SPEED;
    position.y += direction.y * SPEED;
    position.z += direction.z * SPEED;
}

void Planet::takeDmg(int t_damage)
{
    if (t_damage <= 0)
    {
        t_damage = 1;
    }

    if (health > t_damage)
    {
        health -= t_damage;

        // Achievements
        PlanetManager::increaseTimesHit();
        if (t_damage >= 45)
        {
            PlanetManager::increasePowerfulHits();
        }

        mult += t_damage / 20.0f;
        explosionTimer += 0.2f;

        SetSoundPitch(damageSound, 0.5 + static_cast<double>(std::rand()) / RAND_MAX * (0.8 - 0.5));
        PlaySound(damageSound);

        explosionActive = true;
        
        SceneCamera::screenShake(SceneCamera::MEDIUM_SHAKE, 10);
    }
    else
    {
        if (!defeated)
        {
            defeated = true;
            projectiles.clear();

            SceneCamera::screenShake(SceneCamera::LARGE_SHAKE, 30);
            Player::increase3DStage();

            PlaySound(destructionSound);
        }
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

            if (mult > 1.0f)
            {
                mult -= 0.1;
            }
            else
            {
                mult = 1.0f;
            }
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

    displacementIntensity = explosionTimer * mult;

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

void Planet::shoot(Vector3 t_playerPos, Player& t_player)
{
    projectiles.push_back(Projectile(projectileModel, color, position, t_playerPos, t_player));
}

void Planet::shotClock(Vector3 t_playerPos, Player& t_player)
{
    if (fireProjectiles && canShoot)
    {
        if (shootingTimer < DifficultyManager::getOrdinanceSpacing())
        {
            shootingTimer += GetFrameTime();
        }
        else
        {
            shootingTimer = 0.0f;
            shoot(t_playerPos, t_player);
        }
    }
    else if (!canShoot)
    {
        if (cooldownTimer < COOLDOWN_DUR)
        {
            cooldownTimer += GetFrameTime();
        }
        else
        {
            canShoot = true;
            cooldownTimer = 0.0f;
        }
    }
}


void Planet::changeColor(Color t_color)
{
    printf("\n\nCOLOR CHANGE\n\n");
    color = t_color;

    // used to pass to the shader
    Vector4 normalizedColor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
    int colorLocationInShader = GetShaderLocation(explosionShader, "color");
    SetShaderValue(explosionShader, colorLocationInShader, &normalizedColor, SHADER_UNIFORM_VEC4);
}

void Planet::setAlive()
{
    defeated = false; 
    health = maxHealth;

    coreStart = false;
    coreConsumed = false;

    maxRadiusHit = false;
    particleRadius = 30.0f;

    position = originalPos;
}