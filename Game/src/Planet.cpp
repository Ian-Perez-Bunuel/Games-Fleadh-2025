#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include <random>

void Planet::init(Vector3 t_pos)
{
    position = t_pos;
    model = LoadModel("resources/Art/planet.glb");

    roll = 225.0f;
    yaw = 25.0f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
}

void Planet::update()
{
    // roll++;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
}

void Planet::draw()
{
    // DrawModel(model, position, 0.75f, WHITE);
    DrawModelWires(model, position, 0.75f, BLUE);
}


void Planet::takeDmg(int t_damage)
{
    if (health > t_damage)
    {
        health -= t_damage;

        SceneCamera::screenShake(SceneCamera::MEDIUM_SHAKE, 10);

        printf("\n\ndmg taken\n\n");
    }   
}