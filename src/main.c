#include "raylib.h"
#include <string.h>
#include <stdint.h>

#define GS 1000 // GAME SIZE
#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

uint8_t current[GS][GS];
uint8_t next[GS][GS];
uint8_t paused = 1;

Camera2D camera = {0};
RenderTexture2D canvas;

void init()
{
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            int rsg = GetRandomValue(1, 10);
            if(rsg >= 9) current[i][j] = 1;
            else current[i][j] = 0;
        }
    }
}

int neighb(int x, int y)
{
    int count = 0;
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(i == 0 && j == 0) continue;

            int nx = (x + i + GS) % GS;
            int ny = (y + j + GS) % GS;

            if(current[nx][ny] == 1)
            {
                count ++;
            }
        }
    }
    return count;
}

void update()   // modify these rules to get some really fun results
{
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            int count = neighb(i,j);

            if(current[i][j] == 1)
            {
                if(count < 2 || count > 3)  // dying rules
                {
                    next[i][j] = 0;
                }
                else
                {
                    next[i][j] = 1;
                }
            }
            else
            {
                if(count == 3)  // birth rules
                {
                    next[i][j] = 1;
                }
                else next[i][j] = 0;
            }
        }
    }
    memcpy(current, next, sizeof(current));
}

void draw()
{
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            if(current[i][j] == 1)
            {
                DrawPixel(i, j, WHITE);
            }
        }
    }
    EndTextureMode();
}

void movement()
{
    Vector2 mouseDelta = GetMouseDelta();
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) 
    {
        camera.target.x -= mouseDelta.x / camera.zoom;
        camera.target.y -= mouseDelta.y / camera.zoom;
    }
    float scroll = GetMouseWheelMove();
    scroll = clamp(scroll, -10.0f, 10.0f);
    if(scroll != 0)
    {
        camera.zoom += 0.5f * scroll * camera.zoom;
    }
    camera.zoom = clamp(camera.zoom, 1.0f, 10.0f);

    if(IsKeyPressed(KEY_SPACE))
    {
        if(paused == 1) paused = 0;
        else paused = 1;
    }
    if(IsKeyPressed(KEY_R)) init();
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(1280, 720, "Life");

    camera.target = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};
    camera.offset = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    canvas = LoadRenderTexture(GS, GS);

    init();

    while(!WindowShouldClose())
    {
        movement();
        if(paused == 0) update();
        draw();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawTexture(canvas.texture, 0, 0, WHITE);
        EndMode2D();
        DrawFPS(0, 0);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();
}