/*******************************************************************************************
*
*   raylib - Simple Game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_MOBS 20
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

typedef struct
{
    int type;
    Vector3 position;
    Vector3 nextPosition;
    Color color;
    int food;
    float speed;
} Mob;
  
  
//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Natural Life");

    GameScreen currentScreen = GAMEPLAY;

    // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    
    // mobs vars
    Mob mobs[MAX_MOBS];

    for (int i = 0; i < MAX_MOBS; i++)
    {
        int range = 50;
        mobs[i].position = (Vector3){ GetRandomValue(-range, range), 1.0f, GetRandomValue(-range, range) };
        range *= 2;
        mobs[i].nextPosition = (Vector3){ GetRandomValue(-range, range), 1.0f, GetRandomValue(-range, range) };
        mobs[i].food = 100;
        mobs[i].speed = 0.1f;
        
        if (GetRandomValue(0, 1) == 1) {
            mobs[i].type = 1;
            mobs[i].color = (Color){ 255, 153, 51, 255 };
        } else {
            mobs[i].type = 0;
            mobs[i].color = (Color){ 255, 255, 255, 255 };
        }
    }
    
    int wasSetCameraMode = 0;

    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen) 
        {
            case LOGO: 
            {
                framesCounter++;
                
                if (framesCounter > 600)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE: 
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                if (wasSetCameraMode == 0 && IsKeyPressed(KEY_ENTER))
                {
                    //SetCameraMode(camera, CAMERA_FIRST_PERSON);
                    wasSetCameraMode = 1;
                }
                
                if (IsKeyPressed(KEY_E))
                {
                    SetCameraMode(camera, CAMERA_FREE);
                    currentScreen = ENDING;
                }

                for (int i = 0; i < MAX_MOBS; i++)
                {
                    int needNextPosition = 0;
                    
                    if(mobs[i].position.x < mobs[i].nextPosition.x) {
                        mobs[i].position.x += mobs[i].speed;
                    } if(mobs[i].position.x > mobs[i].nextPosition.x) {
                        mobs[i].position.x -= mobs[i].speed;
                    } else {
                        needNextPosition++;
                    }
                    
                    if(mobs[i].position.z < mobs[i].nextPosition.z) {
                        mobs[i].position.z += mobs[i].speed;
                    } if(mobs[i].position.z > mobs[i].nextPosition.z) {
                        mobs[i].position.z -= mobs[i].speed;
                    } else {
                        needNextPosition++;
                    }
                    
                    if (needNextPosition == 2) {
                        mobs[i].nextPosition = (Vector3){ GetRandomValue(-100, 100), 1.0f, GetRandomValue(-100, 100) };
                    }
                }
                
            } break;
            case ENDING: 
            {
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    CloseWindow();
                }  
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            switch(currentScreen) 
            {
                case LOGO: 
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawText("Carregando a simulação", 20, 20, 40, LIGHTGRAY);
                    DrawText("Espere um pouco...", 290, 220, 20, GRAY);
                    
                } break;
                case TITLE: 
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("Natural Life", 20, 20, 40, DARKBLUE);
                    DrawText("PRESSIONE ENTER ou TOQUE para iniciar a simulação", 120, 220, 20, DARKBLUE);
                    
                } break;
                case GAMEPLAY:
                {
                    UpdateCamera(&camera);
                    
                    BeginMode3D(camera);

                    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 10000.0f, 10000.0f }, DARKGREEN); // Draw ground

                    // Draw some cubes around
                    for (int i = 0; i < MAX_MOBS; i++)
                    {
                        DrawCube(mobs[i].position, 1.0f, 1.0f, 2.0f, mobs[i].color);
                    }

                    EndMode3D();
                    
                } break;
                case ENDING: 
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawText("Simulação encerrada", 20, 20, 40, LIGHTGRAY);
                    DrawText("PRESSIONE ENTER ou TOQUE para finalizar a simulação", 120, 220, 20, GRAY);                    
                } break;
                default: break;
            }
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
