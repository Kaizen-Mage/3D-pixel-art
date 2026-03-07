#pragma once
#include "raylib.h"
#include "camera/cameraManager.h"
#include "scene/sceneManager.h"
#include "lighting/lightManager.h"
struct EngineContext{
    CameraManager* cameraManager;
    SceneManager*  sceneManager;
    LightManager*  lightManager; 
};