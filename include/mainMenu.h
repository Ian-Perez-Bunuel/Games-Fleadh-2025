#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "stdio.h"
#include <vector>



class MainMenu
{
public:
    MainMenu();
    ~MainMenu();

    void initialize();
    void update();
    void draw();

private:
    void initializeShaders();

    void input();
    void mouseInput();
    void controllerInput();
};

#endif // MAINMENU_H