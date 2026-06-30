#include "raylib.h"
#include <string.h>
#include <stdint.h>

#define GS 1000 // GAME SIZE
#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

uint8_t current[GS][GS];
uint8_t next[GS][GS];

uint8_t birth_rules[9];
uint8_t survive_rules[9];

uint8_t paused = 1;
int updatewhen = 1;

Camera2D camera = {0};
RenderTexture2D canvas;

int dens = 5;

void init()
{
    for(int i = 0; i < GS; i++)
    {
        for(int j = 0; j < GS; j++)
        {
            int rsg = GetRandomValue(1, 10);
            if(rsg >= dens) current[i][j] = 0;
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

            if(current[nx][ny] == 1)
            {
                count ++;
            }
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
                if(survive_rules[count] == 1)
                {
                    next[i][j] = 1;
                }
                else
                {
                    next[i][j] = 0;
                }
            }
            else
            {
                if(birth_rules[count] == 1)
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

    if(IsKeyPressed(KEY_X) && updatewhen < 81) updatewhen *= 3;
    if(IsKeyPressed(KEY_Z) && updatewhen > 1) updatewhen /= 3;

    int shift = IsKeyDown(KEY_LEFT_SHIFT);

    if(paused == 1)
    {
    if(IsKeyPressed(KEY_ZERO)){if(shift) survive_rules[0] = !survive_rules[0]; else birth_rules[0] = !birth_rules[0];}
    if(IsKeyPressed(KEY_ONE)){if(shift) survive_rules[1] = !survive_rules[1]; else birth_rules[1] = !birth_rules[1];}
    if(IsKeyPressed(KEY_TWO)){if(shift) survive_rules[2] = !survive_rules[2]; else birth_rules[2] = !birth_rules[2];}
    if(IsKeyPressed(KEY_THREE)){if(shift) survive_rules[3] = !survive_rules[3]; else birth_rules[3] = !birth_rules[3];}
    if(IsKeyPressed(KEY_FOUR)){if(shift) survive_rules[4] = !survive_rules[4]; else birth_rules[4] = !birth_rules[4];}
    if(IsKeyPressed(KEY_FIVE)){if(shift) survive_rules[5] = !survive_rules[5]; else birth_rules[5] = !birth_rules[5];}
    if(IsKeyPressed(KEY_SIX)){if(shift) survive_rules[6] = !survive_rules[6]; else birth_rules[6] = !birth_rules[6];}
    if(IsKeyPressed(KEY_SEVEN)){if(shift) survive_rules[7] = !survive_rules[7]; else birth_rules[7] = !birth_rules[7];}
    if(IsKeyPressed(KEY_EIGHT)){if(shift) survive_rules[8] = !survive_rules[8]; else birth_rules[8] = !birth_rules[8];}

    if(IsKeyPressed(KEY_V) && dens < 9)
    {
        dens++;
    }
    if(IsKeyPressed(KEY_C) && dens > 1)
    {
        dens--;
    }
    }

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
        movement();
        frame++;
        if(paused == 0 && frame >= updatewhen)
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

        // 100% there is better way to do that
        if(paused == 1)
        {
        DrawText("BIRTH", 20, 30, 20, GREEN);
        DrawText(birth_rules[0] ? "0:ON" : "0:OFF", 20, 50, 20, GREEN);
        DrawText(birth_rules[1] ? "1:ON" : "1:OFF", 20, 70, 20, GREEN);
        DrawText(birth_rules[2] ? "2:ON" : "2:OFF", 20, 90, 20, GREEN);
        DrawText(birth_rules[3] ? "3:ON" : "3:OFF", 20, 110, 20, GREEN);
        DrawText(birth_rules[4] ? "4:ON" : "4:OFF", 20, 130, 20, GREEN);
        DrawText(birth_rules[5] ? "5:ON" : "5:OFF", 20, 150, 20, GREEN);
        DrawText(birth_rules[6] ? "6:ON" : "6:OFF", 20, 170, 20, GREEN);
        DrawText(birth_rules[7] ? "7:ON" : "7:OFF", 20, 190, 20, GREEN);
        DrawText(birth_rules[8] ? "8:ON" : "8:OFF", 20, 210, 20, GREEN);
        DrawText("SURV", GetScreenWidth()-90, 30, 20, RED);
        DrawText(survive_rules[0] ? "0:ON" : "0:OFF", GetScreenWidth()-90, 50, 20, RED);
        DrawText(survive_rules[1] ? "1:ON" : "1:OFF", GetScreenWidth()-90, 70, 20, RED);
        DrawText(survive_rules[2] ? "2:ON" : "2:OFF", GetScreenWidth()-90, 90, 20, RED);
        DrawText(survive_rules[3] ? "3:ON" : "3:OFF", GetScreenWidth()-90, 110, 20, RED);
        DrawText(survive_rules[4] ? "4:ON" : "4:OFF", GetScreenWidth()-90, 130, 20, RED);
        DrawText(survive_rules[5] ? "5:ON" : "5:OFF", GetScreenWidth()-90, 150, 20, RED);
        DrawText(survive_rules[6] ? "6:ON" : "6:OFF", GetScreenWidth()-90, 170, 20, RED);
        DrawText(survive_rules[7] ? "7:ON" : "7:OFF", GetScreenWidth()-90, 190, 20, RED);
        DrawText(survive_rules[8] ? "8:ON" : "8:OFF", GetScreenWidth()-90, 210, 20, RED);
        DrawText(TextFormat("Density: %d", dens), 20, 600, 20, BLUE);
        }
        DrawText(TextFormat("Speed: %d", updatewhen), 20, 620, 20, BLUE);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();
}