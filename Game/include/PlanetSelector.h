#include "raylib.h"
#include "stdio.h"
#include <math.h>
#include <vector>

#include "../include/PlanetCard.h"

class PlanetSelector
{
public:
    void init();

    bool isActive() { return active; }
    void activate() { active = true; }

    void draw();

    void transition();
private:

    bool active = false;

    Texture2D texture;
    const int SPACING = 600;

    static const int PLANET_AMOUNT = 5;
    std::vector<PlanetCard> planets;
    std::vector<Vector2> startingPos;

    const int TRANSITION_DURATION = 60 * 2;
    int transitionTimer = 0;
    float speed = 5.0f;
};