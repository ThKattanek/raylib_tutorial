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

        // Start the drawing phase for the current frame
        BeginDrawing();

        // Clear the background with a black color
        ClearBackground(BLACK);

        // Get the current FPS
        auto fps = GetFPS();

        // Draw the FPS counter on the screen
        DrawText(TextFormat("FPS: %d", fps), 10, 10, 20, WHITE);

        // End the drawing phase and present the frame on the screen
        EndDrawing();
    }

    // Close the window and clean up resources
    CloseWindow();
    return 0;
}
