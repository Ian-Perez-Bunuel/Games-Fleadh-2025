#include "../include/SceneCamera.h"
#include <random>

// 3D Camera
Camera3D SceneCamera::camera;

float SceneCamera::shakeIntensity = 0.0f;
int SceneCamera::shakeDuration = 0;
bool SceneCamera::shaking = false;

float SceneCamera::offsetX = 0.0f;
float SceneCamera::offsetY = 0.0f;
float SceneCamera::offsetZ = 0.0f;

void SceneCamera::initialize()
{
    camera.position = {0.0f, 0.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};      // Looking at the origin
    camera.up = {0.0f, 1.0f, 0.0f};          // Y-axis is up
    camera.fovy = 45.0f;                     // Field of view in degrees
    camera.projection = CAMERA_PERSPECTIVE;  // 3D perspective projection
}

void SceneCamera::update()
{
    // Apply screen shake effect
    if (shaking)
    {
        if (shakeDuration > 0)
        {
            updateShake();
        }
        else
        {
            shaking = false;
        }
    }

    camera.target.x = offsetX;
    camera.target.y = offsetY;
    camera.target.z = offsetZ;
}

void SceneCamera::updateShake()
{
    offsetX = MAX_OFFSET * shakeIntensity * getRandFloat();
    offsetY = MAX_OFFSET * shakeIntensity * getRandFloat();
    offsetZ = MAX_OFFSET * shakeIntensity * getRandFloat();

    shakeIntensity *= 0.9f;  // Reduce intensity over time
    shakeDuration--;
}

void SceneCamera::screenShake(float t_shake, int t_duration)
{
    shakeIntensity = t_shake / 50.0f;
    shakeDuration = t_duration;
    shaking = true;
}

float SceneCamera::getRandFloat()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    return dis(gen);
}
