#pragma once

#include "raylib.h"
#include "stdio.h"
#include <vector>
#include <memory>

#include "../include/HealthPack.h"
#include "../include/Player.h"

class ObjectManager
{
public:
    ObjectManager(Player& t_player);

    void setObjects();
    std::shared_ptr<Object> findClosestToPlayer();

    void draw();
    void update(Planet& t_planet);

    static const int SMALL;
    static const int MEDIUM;
    static const int LARGE;

private:
    void setRandomTexture();
    Texture2D texture;
    Texture2D texture1;
    Texture2D texture2;
    Texture2D texture3;
    Texture2D texture4;

    const int OBJECT_AMOUNT = 25;
    const int PICKUP_CHANCE = 5; // 1 / 5 chance

    void checkForPickup(std::shared_ptr<Object> t_object);
    
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::weak_ptr<Object>> objectsToSplit;

    Player& player;

    // Pickup info
    void initPickupTextures();
    std::vector<std::shared_ptr<PickUp>> pickups;
    Texture2D healthPackTexture;

    void checkPlayerCollisions(std::shared_ptr<Object>& t_object);
    void checkCollisions();
    void splitObject();

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};