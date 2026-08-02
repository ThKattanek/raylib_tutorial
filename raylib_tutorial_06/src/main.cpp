#include "raylib.h"
#include <cmath>

void LoadAllResources();
void UnloadAllResources();
void DrawScreen(int width, int height);

Texture image;

int main() {

    // Define the screen width and height for the game resolution
    const int screenWidth = 1280;   // HD-Ready Resolution (720p)
    const int screenHeight = 720;

    // Create a window with the specified width, height, and title
    InitWindow(screenWidth, screenHeight, "Raylib 6.0 + CMake");

    // Get the monitor width and height for scaling purposes
    const int monitorWidth = GetMonitorWidth(0);
    const int monitorHeigth = GetMonitorHeight(0);

    // Create a render texture
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    // Set texture filter for the render texture
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    // Initialize the scale variable to 1.0f for no scaling (Windowed mode)
    float scale = 1.0f; // Initialize scale variable to 1.0f for no scaling (Windowed mode)

    // Define the destination rectangle for rendering the texture on the screen
    Rectangle destRec = {
        0,
        0,
        screenWidth * scale,
        screenHeight * scale
    };

    // Set the target frames per second (FPS) for the game loop
    int currentMonitor = GetCurrentMonitor();
    SetTargetFPS(GetMonitorRefreshRate(currentMonitor)); // Set target FPS to the monitor's refresh rate

    // Load all necessary resources for the game (textures, sounds, fonts, etc.)
    LoadAllResources();

    // Main game loop
    while (!WindowShouldClose()) {

        // Start the drawing phase for the current frame
        BeginTextureMode(target);

        // Call the DrawScreen function to render game elements
        DrawScreen(screenWidth, screenHeight);

        // Draw the current FPS on the screen at position (10, 10)
        DrawFPS(10,10);

        // End the drawing phase and present the frame on the screen
        EndTextureMode();

        // Draw target texture to screen
        BeginDrawing();

        // Clear the background with a black color before drawing the texture
        ClearBackground(BLACK);

        // Draw the render texture to the screen with scaling and positioning
        DrawTexturePro(target.texture,
                       Rectangle{ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                       destRec, Vector2{0,0}, 0.0f, WHITE);

        EndDrawing();

        if(IsKeyPressed(KEY_ENTER) && IsKeyDown(KEY_LEFT_ALT))
        {
            currentMonitor = GetCurrentMonitor();
            SetTargetFPS(GetMonitorRefreshRate(currentMonitor)); // Set target FPS to the monitor's refresh rate

            // Toggle fullscreen mode when ALT + ENTER is pressed
            ToggleFullscreen();

            // Update the scale and destination rectangle based on the current window mode (fullscreen or windowed)
            if(IsWindowFullscreen())
            {
                scale = float(monitorWidth) / screenWidth;
                if(float(monitorHeigth) / screenHeight < scale)
                    scale = float(monitorHeigth) / screenHeight;

                destRec = {
                    (monitorWidth - (screenWidth * scale)) * 0.5f,
                    (monitorHeigth - (screenHeight * scale)) * 0.5f,
                    screenWidth * scale,
                    screenHeight * scale
                };
            }
            else
            {
                scale = 1.0f;

                destRec = {0, 0, screenWidth * scale, screenHeight * scale};
            }
        }
    }

    // Unload all resources that were loaded during the game
    UnloadAllResources();

    // Unload the render texture to free up resources
    UnloadRenderTexture(target);

    // Close the window and clean up resources
    CloseWindow();
    return 0;
}

void LoadAllResources()
{
    image = LoadTexture("assets/raylibtech_main.png");
}

void UnloadAllResources()
{
    UnloadTexture(image);
}

void DrawScreen(int width, int height)
{
    static float rotation = 0.0f;
    static float rotationSpeed = 300.0f;
    float deltaTime = GetFrameTime();
    float scale = 1.0f; // Scale factor for the image

    static float timeAccumulator = 0.0f; // Accumulator for time-based scaling

    scale = (sinf(timeAccumulator * PI) + 2) * 0.4f; // Scale the image based on deltaTime and a constant factor

    timeAccumulator += deltaTime; // Update the time accumulator with the elapsed time

    // Clear the background with a black color
    ClearBackground(DARKGRAY);

    // Draw a Texture2D or other game elements here
    DrawTexturePro(image,
                   Rectangle{0, 0, (float)image.width, (float)image.height},    // Source rectangle (the entire image)
                   Rectangle{width * 0.5f, height * 0.5f, (float)image.width * scale, (float)image.height * scale}, // Destination rectangle (centered on the screen with scaling)
                   Vector2{image.width * scale * 0.5f, image.height * scale * 0.5f},    // Set the origin to the center of the scaled image
                   rotation, // Rotate the image by the current rotation angle
                   WHITE); // Draw the image with white tint (no color change)

    rotation += deltaTime * rotationSpeed; // Rotate the image at 45 degrees per second
}
