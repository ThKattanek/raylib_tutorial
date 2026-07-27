#include "raylib.h"

int main() {
    const int screenWidth = 1280;   // HD Ready Resolution
    const int screenHeight = 720;

    // Create a window with the specified width, height, and title
    InitWindow(screenWidth, screenHeight, "Raylib 6.0 + CMake");

    // Set the target frames per second (FPS) for the game loop
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    // Close the window and clean up resources
    CloseWindow();
    return 0;
}
