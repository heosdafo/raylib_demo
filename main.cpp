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
    SetTargetFPS(120);
    SetTraceLogLevel(LOG_WARNING);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
			stage::get_instance()->dispatch_mouse_event();
			stage::get_instance()->update();
			stage::get_instance()->draw();		
			DrawText("Click to explode!", 440, 400, 40, DARKGRAY);
        EndDrawing();
    }
      // Unload sound
    CloseAudioDevice();
    CloseWindow();              // Close window and OpenGL context
    return 0;
}
