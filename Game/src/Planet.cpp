#include "../include/Planet.h"


void Planet::init(Vector3 t_pos)
{
    position = t_pos;
    model = LoadModel("resources/Art/planet1.glb");

    roll = 225.0f;
    pitch = 25.0f;
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
}

void Planet::update()
{
    model.transform = MatrixRotateXYZ({ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });
}

void Planet::draw()
{
    DrawModel(model, position, 0.5f, WHITE);
}