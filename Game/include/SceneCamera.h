#include "raylib.h"
#include "stdio.h"

class SceneCamera
{
public:
    static void initialize();
    static Camera2D camera;

    static void update();

    static void screenShake(float t_shake, int t_duration);
    
    // Pre-set Shake values
    static const int SMALL_SHAKE = 2.0f;
    static const int MEDIUM_SHAKE = 5.0f;
    static const int LARGE_SHAKE = 15.0f;

private:

    static const int MAX_ANGLE = 1;

    static float shakeIntensity;
    static int shakeDuration;
    static bool shaking;


    static float getRandFloat();
};