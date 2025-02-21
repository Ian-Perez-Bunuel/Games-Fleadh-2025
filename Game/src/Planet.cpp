#include "../include/Planet.h"
#include "../include/SceneCamera.h"
#include <random>
#include "rlgl.h"

void Planet::init(Vector3 t_pos, Color t_color)
{
    position = t_pos;
    model = LoadModel("resources/Art/3D/planet4.glb");
    core = LoadModel("resources/Art/3D/core.glb");
    shield = LoadModel("resources/Art/3D/shield.glb");

    color = t_color;
}

void Planet::update()
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

    // rlSetLineWidth(10.0f);
    // DrawModel(shield, position, 1.0, WHITE);
    // DrawModelWires(shield, position, 1.0, BLUE);
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