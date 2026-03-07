#include "raylib.h"
#include "raymath.h"
#include "engine/engineContext.h"
#include "state/mainstate.h"
int main() {

    InitWindow(1920, 1080, "3D pixel art");
    ToggleFullscreen();
    EngineContext context;
    //Camera
    CameraManager cameraManager;
    cameraManager.Initialize();
    context.cameraManager=&cameraManager;
    //Scene
    SceneManager sceneManager;
    context.sceneManager=&sceneManager;
    //Lighting
    LightManager lightManager;
    GameState state=GameState(&context);
    while (!WindowShouldClose()) {
        float dt=GetFrameTime();
        state.Update(dt);
        state.Draw();
    }
    
    CloseWindow();
}