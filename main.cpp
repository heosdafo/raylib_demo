#include <raylib.h>
#include "stage.h"
#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite explosion");
    InitAudioDevice();
    Sound fxBoom = LoadSound("boom.wav");
    Texture2D explosion = LoadTexture("explosion.png");
    SetTargetFPS(120);
    SetTraceLogLevel(LOG_WARNING);
	stage* Stage = new stage(); 
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
			Stage->dispatch_mouse_event();
			Stage->update();
			Stage->draw();
			
			DrawText("Click to explode!", 440, 400, 40, DARKGRAY);
        EndDrawing();
    }
    UnloadTexture(explosion);   // Unload texture
    UnloadSound(fxBoom);        // Unload sound
    CloseAudioDevice();
    CloseWindow();              // Close window and OpenGL context
    return 0;
}
