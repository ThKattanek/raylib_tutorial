#include "raylib.h"
#include <cmath>
#include <cstdint>
#include <vector>

void LoadAllResources();
void UnloadAllResources();
void DrawScreen(int width, int height);

struct IntPoint{
    int x,y;
    Color color;
};

std::vector<IntPoint> points;

IntPoint currentPoint;
int currentDirection = 1; // 0 = up, 1 = right, 2 = down, 3 = left

int main() {

    // Define the screen width and height for the game resolution
    const int screenWidth = 1280;   // HD-Ready Resolution (720p)
    const int screenHeight = 720;

    currentPoint = { int(screenWidth * 0.5f), int(screenHeight * 0.5f)};

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
}

void UnloadAllResources()
{
    points.clear();
}

void DrawScreen(int width, int height)
{
    // Clear the background with a black color
    ClearBackground(DARKGRAY);

    //
    int notDirection;

    static int counter = 0;
    static int count_max = GetRandomValue(1, 50);

    if(currentDirection == 0) // Up
        notDirection = 2;
    else if(currentDirection == 1) // Right
        notDirection = 3;
    else if(currentDirection == 2) // Down
        notDirection = 0;
    else // Left
        notDirection = 1;

    // Generate a random direction (0 to 3) that is different from the current direction
    if(counter == 0)
    {
        do{
            currentDirection = GetRandomValue(0,3);
        }while(currentDirection == notDirection);

        count_max = GetRandomValue(1, 50); // Randomize the number of steps in the current direction
    }
        // Move the current point based on the chosen direction
        if(currentDirection == 0) // Up
            currentPoint.y--;
        else if(currentDirection == 1) // Right
            currentPoint.x++;
        else if(currentDirection == 2) // Down
            currentPoint.y++;
        else // Left
            currentPoint.x--;

        if(currentPoint.x >= width)
            currentPoint.x = 0;
        if(currentPoint.x < 0)
            currentPoint.x = width-1;

        if(currentPoint.y >= height)
            currentPoint.y = 0;
        if(currentPoint.y < 0)
            currentPoint.y = height-1;

    counter++;
    if(counter > count_max)
        counter = 0;

    // Draw all Points
    for(auto &p : points)
    {
        DrawPixel(p.x, p.y, p.color);
    }

    // Add the current point to the list of points
    static float time = GetFrameTime();
    uint8_t c = sin(time * 10.0f) * 127 + 128; // Calculate a color value based on the sine of the frame time
    currentPoint.color = Color{c, c, c, 255}; // Set the color of the current point to white
    points.push_back(currentPoint);

    time+= GetFrameTime();

    if(points.size() > 5000)
    {
        if (!points.empty()) {
            points.erase(points.begin());
        }
    }

    DrawPixel(currentPoint.x, currentPoint.y, BLACK);
    DrawCircleLines(currentPoint.x, currentPoint.y, 10, BLACK);
    DrawText(TextFormat("%d, %d", currentPoint.x, currentPoint.y), currentPoint.x - 20, currentPoint.y - 25, 10, BLACK);

    DrawText( TextFormat("Current draw pixles for frame: %d", points.size()),10, height - 25, 20, WHITE);
}
