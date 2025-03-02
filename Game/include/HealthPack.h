#pragma once

#include "raylib.h"
#include "stdio.h"

#include "../include/PickUp.h"

class HealthPack : public PickUp
{
public:
    HealthPack(Texture2D& t_texture, Player& t_player, std::shared_ptr<Object> t_parentObject, int t_radius);

    void draw() override;

    void onPickup() override;

private:
    Texture2D texture;

    const int HEAL_AMOUNT = 20;

    Sound pickupSound;
};