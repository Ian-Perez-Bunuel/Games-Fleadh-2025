#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include <random>
#include "rlgl.h"

void Planet::init(Vector3 t_pos, Color t_color)
{
    position = t_pos;
    model = LoadModel("resources/Art/3D/planet1.glb");
    core = LoadModel("resources/Art/3D/core.glb");

    roll = 45.0f;
    yaw = 25.0f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });

    color = t_color;
}

void Planet::update()
{
    yaw += 0.5f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
    core.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
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
}


void Planet::takeDmg(int t_damage)
{
    if (health > t_damage)
    {
        health -= t_damage;

        SceneCamera::screenShake(SceneCamera::MEDIUM_SHAKE, 10);
    }
    else
    {
        defeated = true;
    }  
}