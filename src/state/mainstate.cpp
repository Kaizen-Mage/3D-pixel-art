#include "state/mainState.h"
GameState::GameState(EngineContext* ctx):context(ctx){
    context->sceneManager->tex=LoadRenderTexture(480,270);

}
void GameState::Destroy(){

}
void GameState::Update(float dt){
    context->cameraManager->EnableFreeLook(20.0f,dt);
    context->cameraManager->UpdateInternalCamera();
}
void GameState::Draw(){
     BeginDrawing();
        ClearBackground(BLACK);
        BeginTextureMode(context->sceneManager->tex);
        ClearBackground(BLACK);
        context->cameraManager->BeginCamera();
        DrawGrid(100,10);
        DrawCube(Vector3Zero(),1.0f,2.0f,1.0f,WHITE);
        DrawCube({0.0f,-1.0f,0.0f},4.0f,1.0f,4.0f,RED);
        context->cameraManager->EndCamera();
        EndTextureMode();
        DrawTexturePro(context->sceneManager->tex.texture,
        {0,0,(float)context->sceneManager->tex.texture.width,(float)-context->sceneManager->tex.texture.height},
        {0,0,1920,1080},{0.0},0.0f,WHITE);
        DrawFPS(20,20);
        EndDrawing();
}