#include "lighting/lightManager.h"
int maxLights=10;
int lightsCount=0;
// Send light properties to shader
// NOTE: Light shader locations should be available 
void UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}

// Create a light and get shader locations
Light  CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    
    Light light = { 0 };

    if (lightsCount<maxLights)
    {
        light.enabled = true;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color = color;

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));

        UpdateLightValues(shader, light);
        
        lightsCount++;
    }

    return light;
}


LightManager::LightManager(){
    gbuffer=LoadShader("include/resources/shaders/lighting/gbuffer.vs","include/resources/shaders/lighting/gbuffer.fs");
    deferredShader=LoadShader("include/resources/shaders/lighting/deferredShading.vs","include/resources/shaders/lighting/deferredShading.fs");
    
   
    //LOAD A BUFFER WHERE YOU CAN WRITE TO
    gBuffer.framebufferId = rlLoadFramebuffer();
    if (gBuffer.framebufferId == 0) {
        std::cout<< "Failed to create framebufferId"<<std::endl;
    }
    else{
        std::cout<<"FrameBuffer loaded successfully"<<std::endl;
    }
    //Tell opengl to use this buffer and not the screen
    rlEnableFramebuffer(gBuffer.framebufferId);
    //LOAD textures for positions normals and albedos
    gBuffer.positionTextureId=rlLoadTexture(NULL,renderWidth,renderHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,1);
    gBuffer.normalTextureId=rlLoadTexture(NULL,renderWidth,renderHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,1);
    gBuffer.albedoSpecTextureId=rlLoadTexture(NULL,renderWidth,renderHeight,RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,1);
    gBuffer.depthTextureId=rlLoadTexture(NULL,renderWidth,renderHeight,RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,1);
    //tell opengl that it can write to these three textures
    rlActiveDrawBuffers(4);
    rlFramebufferAttach(gBuffer.framebufferId,gBuffer.positionTextureId,RL_ATTACHMENT_COLOR_CHANNEL0,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.framebufferId,gBuffer.normalTextureId,RL_ATTACHMENT_COLOR_CHANNEL1,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.framebufferId,gBuffer.albedoSpecTextureId,RL_ATTACHMENT_COLOR_CHANNEL2,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.framebufferId,gBuffer.depthTextureId,RL_ATTACHMENT_COLOR_CHANNEL3,RL_ATTACHMENT_TEXTURE2D,0);
    //LOAD A RENDER BUFFER FOR DEPTH TESTING
    gBuffer.depthRenderbufferId = rlLoadTextureDepth(renderWidth, renderHeight, true);
    rlFramebufferAttach(gBuffer.framebufferId, gBuffer.depthRenderbufferId, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    if(rlFramebufferComplete(gBuffer.framebufferId)){
        std::cout<<"The frameBuffer is complete"<<std::endl;
    }
    else{
        std::cout<<"The framebuffer is incomplete"<<std::endl;
    }

    //Set ids for deferred shader
    rlEnableShader(deferredShader.id);
    SetShaderValue(deferredShader,rlGetLocationUniform(deferredShader.id,"gPosition"),&texUnitPosition,RL_SHADER_UNIFORM_INT);
    SetShaderValue(deferredShader,rlGetLocationUniform(deferredShader.id,"gNormal"),&texUnitNormal,RL_SHADER_UNIFORM_INT);
    SetShaderValue(deferredShader,rlGetLocationUniform(deferredShader.id,"gAlbedoSpec"),&texUnitAlbedoSpec,RL_SHADER_UNIFORM_INT);
    rlDisableShader();
    
    CreateLight(0,{-2,-2,-2},{0.0,0},RED,deferredShader);
}
LightManager::~LightManager(){

}