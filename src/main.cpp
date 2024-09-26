#include "raylib.h"
#include "Math.h"

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);

    float speed = 100.0f;
    float position = 1.0f;

    while (!WindowShouldClose())
    {
        float time = GetFrameTime();
        position += speed * time;
        Vector2 mousepos = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 10, 10, 20, GRAY);

        DrawCircleV(mousepos, 25, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
