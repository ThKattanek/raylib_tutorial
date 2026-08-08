#include "raylib.h"
#include "raymath.h"

// Aktiviert die Licht-Hilfsfunktionen von Raylib
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

int main(void)
{
    InitWindow(1920, 1080, "Raylib - 3D Modell mit Licht");

    Camera camera = { {0} };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // 1. Modell laden
    Model myModel = LoadModel("assets/3d_objects/test1.glb");


    // --- NEU: Animationen laden ---
    int animCount = 0;
    // Wir laden die Animationsdaten aus derselben Datei
    ModelAnimation *anims = LoadModelAnimations("assets/3d_objects/test1.glb", &animCount);
    int animFrameCounter = 0; // Zähler für den aktuellen Frame
    // ------------------------------

    // 2. Shader laden (Pfade ggf. anpassen, wenn sie in einem Unterordner liegen)
    Shader shader = LoadShader("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");

    // WICHTIG: Dem Shader die Eigenschaft "locs" (Locations) für View-Position und Ambient-Licht zuweisen
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Umgebungslicht (Ambient) festlegen, damit die Schattenseite nicht komplett pechschwarz ist
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Dunkles Grau
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    // 3. Den Shader auf ALLE Materialien des Modells anwenden
    for (int i = 0; i < myModel.materialCount; i++)
    {
        myModel.materials[i].shader = shader;
    }

    // 4. Licht erstellen (Punktlicht an Position 10, 20, 10)
    Light light = CreateLight(LIGHT_POINT, (Vector3){ 10.0f, 20.0f, 10.0f }, Vector3Zero(), WHITE, shader);

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);


        // --- NEU: Animation updaten ---
        // Wir prüfen sicherheitshalber, ob überhaupt eine Animation in der Datei gefunden wurde
        if (animCount > 0)
        {
            animFrameCounter++;

            // Wenn der Zähler das Ende der ersten Animation (Index 0) erreicht, setze ihn auf 0 zurück (Loop)
            if (animFrameCounter >= anims[0].keyframeCount)
            {
                animFrameCounter = 0;
            }

            // Wendet die Deformation für den aktuellen Frame auf das Gittermodell an
            UpdateModelAnimation(myModel, anims[0], animFrameCounter);
        }
        // ------------------------------


        // 5. Kamera-Position an den Shader senden (für Glanzpunkte auf dem Modell)
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        // Update der Lichtwerte (falls sich das Licht bewegen würde)
        UpdateLightValues(shader, light);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawModel(myModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);

        // Eine kleine Kugel zeichnen, um zu sehen, wo das Licht ist
        DrawSphere(light.position, 0.5f, YELLOW);

        DrawGrid(10, 1.0f);
        EndMode3D();

        DrawFPS(10,10);

        // Wenn eine Animation gefunden wurde, zeigen wir zur Kontrolle den aktuellen Frame an
        if (animCount > 0) {
            DrawText(TextFormat("Animation Frame: %d / %d", animFrameCounter, anims[0].keyframeCount), 10, 40, 20, RED);
        }

        EndDrawing();
    }

    UnloadShader(shader);
    UnloadModel(myModel);
    CloseWindow();

    return 0;
}