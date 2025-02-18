#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include <random>
#include "rlgl.h"

void Planet::init(Vector3 t_pos)
{
    position = t_pos;
    model = LoadModel("resources/Art/3D/planet1.glb");

    roll = 45.0f;
    yaw = 25.0f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });

    rlSetLineWidth(2.0f);
}

void Planet::update()
{
    yaw += 0.5f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
}

void Planet::draw()
{
    DrawModelWires(model, position, 0.75f, RED);
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