#include "../include/HealthPack.h"
#include <random>


HealthPack::HealthPack(Texture2D& t_texture, Player& t_player, std::shared_ptr<Object> t_parentObject, int t_radius)
    : texture(t_texture), PickUp(t_player, t_parentObject, t_radius)
{
    particleSpawner.addColor(GREEN);
    pickupSound = LoadSound("resources/Sound/heal.mp3");
    SetSoundVolume(pickupSound, 0.2f);
}

void HealthPack::draw()
{
    if (!pickedUp)
    {
        // DrawCircleV(position, radius, WHITE);
        DrawTextureEx(texture, {position.x - (radius * 1.3f), position.y - (radius * 1.3f)}, 0, 0.5f, WHITE);
    }

    particleSpawner.draw();
}


void HealthPack::onPickup()
{
    SetSoundPitch(pickupSound, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
    PlaySound(pickupSound);

    player.heal(HEAL_AMOUNT);

    particleSpawner.setValues(position, 360, 0);
    particleSpawner.spawn();
}