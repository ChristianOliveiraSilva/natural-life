#include <math.h>
#include "raylib.h"

#define MAX_MOBS 20
#define MAX_GRASS 10

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

typedef struct
{
    int type;
    Vector3 position;
    Vector3 nextPosition;
    Color color;
    int food;
    int life;
    float speed;
} Mob;

typedef struct
{
    int show;
    Vector3 position;
} Grass;

float calculateDistance(float x1, float z1, float x2, float z2)  {
    float x = x2 - x1;
    float z = z2 - z1;
    
    return sqrt(pow(x, 2) + pow(z, 2));
}

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "Natural Life");

    GameScreen currentScreen = GAMEPLAY;
    DisableCursor();

    int framesCounter = 0;
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // mobs vars
    Grass scrub[MAX_MOBS]; 
    Mob mobs[MAX_MOBS];   
    int range = 50;

    for (int i = 0; i < MAX_GRASS; i++)
    {
        scrub[i].show = 1;
        scrub[i].position = (Vector3){ GetRandomValue(-range, range), 1.0f, GetRandomValue(-range, range) };
    }
 
    for (int i = 0; i < MAX_MOBS; i++)
    {
        mobs[i].position = (Vector3){ GetRandomValue(-range, range), 1.0f, GetRandomValue(-range, range) };
        mobs[i].nextPosition = (Vector3){ GetRandomValue(-range, range), 1.0f, GetRandomValue(-range, range) };
        mobs[i].food = 100;
        mobs[i].life = 100;
        
        if (i % 2 != 0) { // lobo
            mobs[i].type = 1;
            mobs[i].color = (Color){ 255, 153, 51, 255 };
            mobs[i].speed = 0.014f;
        } else { // ovelha
            mobs[i].type = 0;
            mobs[i].color = (Color){ 255, 255, 255, 255 };
            mobs[i].speed = 0.01f;
        }
    }
    
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        framesCounter = framesCounter >= 24000 ? 0 : framesCounter+1;
        
        switch(currentScreen) 
        {
            case LOGO: 
            {
                if (framesCounter > 600)
                {
                    currentScreen = TITLE;
                    DisableCursor();
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
                if (IsKeyPressed(KEY_E))
                {
                    currentScreen = ENDING;
                }

                for (int i = 0; i < MAX_MOBS; i++)
                {                    
                    if(mobs[i].position.x < mobs[i].nextPosition.x) {
                        mobs[i].position.x += mobs[i].speed;
                    } if(mobs[i].position.x > mobs[i].nextPosition.x) {
                        mobs[i].position.x -= mobs[i].speed;
                    }
                    
                    if(mobs[i].position.z < mobs[i].nextPosition.z) {
                        mobs[i].position.z += mobs[i].speed;
                    } if(mobs[i].position.z > mobs[i].nextPosition.z) {
                        mobs[i].position.z -= mobs[i].speed;
                    }
                    
                    if (calculateDistance(mobs[i].position.x, mobs[i].position.z, mobs[i].nextPosition.x, mobs[i].nextPosition.z) < 1) {
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
        
            ClearBackground(SKYBLUE);
            
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
                    UpdateCamera(&camera, CAMERA_FIRST_PERSON);
                    
                    BeginMode3D(camera);

                    DrawPlane((Vector3){ camera.position.x, 0, camera.position.z }, (Vector2){ 10000.0f, 10000.0f }, DARKGREEN);

                    for (int i = 0; i < MAX_GRASS; i++)
                    {
                        if (scrub[i].show == 1) {
                            float tall = i % 3 == 0 ? 1.0f : 0.7f;
                            DrawCube(scrub[i].position, 0.2f, tall, 0.2f, GREEN);
                        }
                    }

                    for (int i = 0; i < MAX_MOBS; i++)
                    {
                        DrawCube(mobs[i].position, 0.8f, 0.8f, 1.6f, mobs[i].color);
                    }

                    EndMode3D();
                    
                    DrawText("PRESSIONE E para finalizar a simulação", screenWidth * 0.65, screenHeight - 25, 20, DARKBLUE);
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
