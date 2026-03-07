#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "shared/shared.h"
#include <iostream>
struct GBuffer{
    unsigned int framebufferId;
    unsigned int positionTextureId;
    unsigned int normalTextureId;
    unsigned int albedoSpecTextureId;
    unsigned int depthTextureId;
    unsigned int depthRenderbufferId;
   
};
struct Light{
    int type;
    bool enabled;
    Vector3 position;
    Vector3 target;
    Color color;
    float attenuation;
    
    // Shader locations
    int enabledLoc;
    int typeLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int attenuationLoc;
};
enum class DeferredMode{
    DEFERRED_POSITION,
    DEFERRED_NORMAL,
    DEFERRED_ALBEDO,
    DEFERRED_SHADING,
    DEFERRED_DEPTH
};
class LightManager
{
private:
   
public:
    int texUnitPosition   = 0;
    int texUnitNormal     = 1;
    int texUnitAlbedoSpec = 2;
    int maxLights=10;
    int lightsCount=0;
    GBuffer gBuffer;
    Shader gbuffer;
    Shader deferredShader;
    Light lights[5];
    LightManager();
    ~LightManager();
};
