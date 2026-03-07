#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>
class RenderCamera{
private:
public:
    Camera3D cam;
    Vector3 position;
    Vector3 target;
    Vector3 up;
    float fovy=45.0f;
    float yaw=0.0f;
    float pitch=0.0f;
    float orbitRadius=10.0f;
    float sensitivity=0.25f;
    CameraProjection projection;
    int cameraMode=CAMERA_FREE;
    RenderCamera();
    RenderCamera(Vector3 position,Vector3 target,Vector3 up,CameraProjection projection);
    void Update();
    void Follow(Vector3 followTarget);
    void MouseMove();
    void FreeLook(float moveSpeed,float dt);
    void Orbit(float moveSpeed,float dt);
    void LookAt(Vector3 targetPosition);
    void Begin();
    void End();
};