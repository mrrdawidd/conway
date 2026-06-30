#include "raylib.h"
#include <string.h>
#include <stdint.h>

#define GS 1000
#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

uint8_t current[GS][GS];
uint8_t next[GS][GS];

uint8_t birth_rules[9];
uint8_t survive_rules[9];

uint8_t paused = 1;
int speed = 1;
int dens = 5;

Camera2D camera = {0};
RenderTexture2D canvas;

void init()
{
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            int rsg = GetRandomValue(1, 11);
            if(rsg > dens) current[i][j] = 0;
            else current[i][j] = 1;
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

            if(current[nx][ny] == 1) count ++;
        }
    }
    return count;
}

void update()
{
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            int count = neighb(i,j);

            if(current[i][j] == 1)
            {
                if(survive_rules[count] == 1) next[i][j] = 1;
                else next[i][j] = 0;
            }
            else
            {
                if(birth_rules[count] == 1) next[i][j] = 1;
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
            if(current[i][j] == 1) DrawPixel(i, j, WHITE);
        }
    }
    EndTextureMode();
}

void controls()
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

    if(IsKeyPressed(KEY_Z) && speed < 64) speed *= 2;
    if(IsKeyPressed(KEY_X) && speed > 1) speed /= 2;

    if(IsKeyPressed(KEY_C) && dens > 1) dens--;
    if(IsKeyPressed(KEY_V) && dens < 10) dens++;

    int shift = IsKeyDown(KEY_LEFT_SHIFT);

    if(IsKeyPressed(KEY_ZERO))  {if(shift) survive_rules[0] = !survive_rules[0]; else birth_rules[0] = !birth_rules[0];}
    if(IsKeyPressed(KEY_ONE))   {if(shift) survive_rules[1] = !survive_rules[1]; else birth_rules[1] = !birth_rules[1];}
    if(IsKeyPressed(KEY_TWO))   {if(shift) survive_rules[2] = !survive_rules[2]; else birth_rules[2] = !birth_rules[2];}
    if(IsKeyPressed(KEY_THREE)) {if(shift) survive_rules[3] = !survive_rules[3]; else birth_rules[3] = !birth_rules[3];}
    if(IsKeyPressed(KEY_FOUR))  {if(shift) survive_rules[4] = !survive_rules[4]; else birth_rules[4] = !birth_rules[4];}
    if(IsKeyPressed(KEY_FIVE))  {if(shift) survive_rules[5] = !survive_rules[5]; else birth_rules[5] = !birth_rules[5];}
    if(IsKeyPressed(KEY_SIX))   {if(shift) survive_rules[6] = !survive_rules[6]; else birth_rules[6] = !birth_rules[6];}
    if(IsKeyPressed(KEY_SEVEN)) {if(shift) survive_rules[7] = !survive_rules[7]; else birth_rules[7] = !birth_rules[7];}
    if(IsKeyPressed(KEY_EIGHT)) {if(shift) survive_rules[8] = !survive_rules[8]; else birth_rules[8] = !birth_rules[8];}
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(1280, 720, "Life");

    camera.target = (Vector2){GS/2, GS/2};
    camera.offset = (Vector2){GetScreenWidth()/2, GetScreenHeight()/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    canvas = LoadRenderTexture(GS, GS);

    int frame = 0;

    init();
    
    for(int i = 0; i < 9; i++)
    {
        birth_rules[i] = 0;
        survive_rules[i] = 0;
    }

    birth_rules[3] = 1;
    survive_rules[2] = 1;
    survive_rules[3] = 1;

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        controls();
        frame++;

        if(paused == 0 && frame >= speed)
        {
            update();
            frame = 0;
        }

        draw();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawTexture(canvas.texture, 0, 0, WHITE);
        EndMode2D();
        DrawFPS(0, 0);

        char birth[10] = "";
        int b = 0;
        for(int i = 0; i <= 8; i++)
        {
            if(birth_rules[i])
            {
                birth[b] = '0' + i;
                b++;
            }
        }
        birth[b] = '\0';

        char survive[10] = "";
        int s = 0;
        for(int i = 0; i <= 8; i++)
        {
            if(survive_rules[i])
            {
                survive[s] = '0' + i;
                s++;
            }
        }
        survive[s] = '\0';

        DrawText(TextFormat("B: %s/S: %s", birth, survive), 0, 20, 20, BLUE);
        DrawText(TextFormat("Speed: %d", 64 / speed), 0, 40, 20, RED);
        DrawText(TextFormat("Density: %d", dens), 0, 60, 20, ORANGE);

        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();
}