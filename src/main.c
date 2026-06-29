#include "raylib.h"
#define GWIDTH 1000
#define GHEIGHT 1000

int current[GWIDTH][GHEIGHT];
int next[GWIDTH][GHEIGHT];

void init()
{
    for(int i = 1; i < GWIDTH-1; i++)
    {
        for(int j = 1; j < GHEIGHT-1; j++)
        {
            current[i][j] = GetRandomValue(0, 1);
        }
    }
}

int neighb(int x, int y)
{
    int count = 0;
    for(int i = -1; i<=1; i++)
    {
        for(int j = -1; j<=1; j++)
        {
            if(i == 0 && j == 0) continue;

            int nx = i + x;
            int ny = j + y;
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
    
    for(int i = 1; i < GWIDTH-1; i++)
    {
        for(int j = 1; j < GHEIGHT-1; j++)
        {
            int count = neighb(i,j);
            if(current[i][j] == 1)
            {
                if(count < 2 || count > 3)
                {
                    next[i][j] = 0;
                }
                else
                {
                    next[i][j] = 1;
                }
            }
            else{
                if(count == 3)
                {
                    next[i][j] = 1;
                }
                else next[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < GWIDTH; i++) 
    {
        for(int j = 0; j < GHEIGHT; j++) 
        {
            current[i][j] = next[i][j];
        }
    }
}

void draw()
{
    for(int i = 0; i < GWIDTH; i++)
    {
        for(int j = 0; j < GHEIGHT; j++)
        {
            if(current[i][j] == 1)
            {
                DrawPixel(i, j, WHITE);
            }
        }
    }
}

int main()
{
    InitWindow(1000, 1000, "Test");
    SetTargetFPS(60);
    init();
    Camera2D camera = {0};
    camera.target = (Vector2){500.0f, 500.0f};
    camera.offset = (Vector2){500.0f, 500.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while(!WindowShouldClose())
    {
        update();
        if(IsKeyPressed(KEY_E) && camera.zoom < 5.0f) camera.zoom += 1.0f;
        if(IsKeyPressed(KEY_Q) && camera.zoom > 1.0f) camera.zoom -= 1.0f;
        if(IsKeyDown(KEY_A)) camera.target.x -= 5.0f;
        if(IsKeyDown(KEY_D)) camera.target.x += 5.0f;
        if(IsKeyDown(KEY_W)) camera.target.y -= 5.0f;
        if(IsKeyDown(KEY_S)) camera.target.y += 5.0f;
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        draw();
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}