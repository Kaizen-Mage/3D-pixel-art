#pragma once
#include "camera/camera.h"
class CameraManager{
public:
    RenderCamera camera;
    void Initialize();
    void EnableFreeLook(float moveSpeed,float dt);
    void EnableOrbit(float moveSpeed,float dt);
    void UpdateInternalCamera();
    void BeginCamera();
    void EndCamera();
};