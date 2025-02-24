#include "raylib.h"
#include "stdio.h"

class SceneCamera
{
public:
    static void initialize();
    static Camera3D camera;

    static void update();
    static void screenShake(float t_shake, int t_duration);

    static void moveToNextPlanet();

    // Pre-set Shake values
    static const int SMALL_SHAKE = 2.0f;
    static const int MEDIUM_SHAKE = 5.0f;
    static const int LARGE_SHAKE = 15.0f;

private:
    static void updateShake();

    static float shakeIntensity;
    static int shakeDuration;
    static bool shaking;

    static float offsetX;
    static float offsetY;
    static float offsetZ;

    static const int MAX_OFFSET = 1.2;

    static float getRandFloat();
};
