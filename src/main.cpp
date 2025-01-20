#include "raylib.h"
#include "stdio.h"

#include "../include/Globals.h"
#include "../include/game.h"

// Specific include for build_web
#if defined(WEB_BUILD)
#include <emscripten/emscripten.h>
#endif



void run();
// Initialise Game
Game game;

int main(void)
{

    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Raylib StarterKit GPPI");

    // For web builds, do not use WindowShouldClose
    // see https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#41-avoid-raylib-whilewindowshouldclose-loop

#if defined(WEB_BUILD)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Call GameLoop
        run();
    }
#endif

    CloseWindow();

    return 0;
}

int counter = 0;
char message[11];

void run()
{
    // Update Game Data
    // Should be outside BeginDrawing(); and EndDrawing();
    game.update();

    BeginDrawing();
    ClearBackground(BLACK);

    game.draw();

    EndDrawing();
}
