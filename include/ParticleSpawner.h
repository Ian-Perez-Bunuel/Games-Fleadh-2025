#include "../include/Particle.h"

class ParticleSpawner
{
public:
    void setValues(Vector2 t_pos, int t_angleLeeway, float t_dirAngle);

    void spawn();
    void addColor(Color t_color);

    void update();
    void draw();

private:
    bool checkIfParticalsActive();
    // Randomizing functions
    float randomizeDir();
    Color randomizeColor();

    Vector2 position;

    const int MAX_PARTICLES = 10;
    std::vector<Particle> particles;

    int angleLeeway;
    float dirAngle;

    std::vector<Color> colors;

    bool active = false;
};