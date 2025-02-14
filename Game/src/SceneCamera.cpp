#include "../include/SceneCamera.h"
#include <random>
#include "../include/Globals.h"
// Orthogonal
Camera2D SceneCamera::camera;

float SceneCamera::shakeIntensity = 0.0f;
int SceneCamera::shakeDuration = 0;
bool SceneCamera::shaking = false;

void SceneCamera::initialize()
{
    camera.target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera.offset = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void SceneCamera::update()
{
    // Apply screenshake effect
    if (shaking)
    {
        if (shakeDuration > 0)
        {
            camera.offset.x = SCREEN_WIDTH / 2 + getRandFloat() * shakeIntensity;
            camera.offset.y = SCREEN_HEIGHT / 2 + getRandFloat() * shakeIntensity;
            camera.rotation = 0.2f * shakeIntensity * getRandFloat();

            shakeIntensity *= 0.9f;  // Reduce intensity over time
            shakeDuration--;
        }
        else
        {
            shaking = false;
            camera.offset.x = SCREEN_WIDTH / 2;
            camera.offset.y = SCREEN_HEIGHT / 2;
            camera.rotation = 0;
        }
    }
}

void SceneCamera::screenShake(float t_shake, int t_duration)
{
    shakeIntensity = t_shake;
    shakeDuration = t_duration;

    shaking = true;
}


float SceneCamera::getRandFloat()
{
    static std::random_device rd;  // Seed for the random number engine
    static std::mt19937 gen(rd()); // Standard Mersenne Twister engine
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f); // Range [-1, 1]

    return dis(gen);
}