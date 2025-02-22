#pragma once

#include "raylib.h"
#include "stdio.h"
#include "memory"

#include "../include/Player.h"
#include "../include/Object.h"
#include "../include/ParticleSpawner.h"

class PickUp
{
public:
    PickUp(Player& t_player, std::shared_ptr<Object> t_parentObject, int t_radius) : player(t_player), parentObject(t_parentObject), radius(t_radius)
    {}
    
    virtual void draw() = 0;
    virtual void update();

    void setActive() { active = true; }
    
protected:
    virtual void onPickup() = 0;

    std::shared_ptr<Object> parentObject = nullptr;
    Player& player;

    bool pickedUp = false;
    bool active = false;

    Vector2 position;
    int radius;

    ParticleSpawner particleSpawner;

private:
    void collisionDetection();  

    float pointToPointDist(Vector2 t_p1, Vector2 t_p2) { return sqrt(((t_p2.x - t_p1.x) * (t_p2.x - t_p1.x)) + ((t_p2.y - t_p1.y) * (t_p2.y - t_p1.y))); }
};