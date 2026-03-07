#include "camera/cameraManager.h"
void CameraManager::EnableFreeLook(float moveSpeed,float dt){
    camera.FreeLook(moveSpeed,dt);
}
void CameraManager::EnableOrbit(float moveSpeed,float dt){
    camera.Orbit(moveSpeed,dt);
}
void CameraManager::Initialize(){
    camera.position={0.0f,2.0f,-4.0f};
    camera.target={0.0f,0.0f,0.0f};
    camera.fovy=45.0f;
    camera.up={0.0f,1.0f,0.0f};
    camera.projection=CAMERA_PERSPECTIVE;
}
void CameraManager::UpdateInternalCamera(){
    camera.Update();
}
void CameraManager::BeginCamera(){
    camera.Begin();
}
void CameraManager::EndCamera(){
    camera.End();
}