#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "rlights.h"
#include <iostream>
int screenWidth=1920;
int screenHeight=1080;
int downWidth=1920/4;
int downHeight=1080/4;
struct GBuffer{
    unsigned int frameBufferId;
    unsigned int positionTextureId;
    unsigned int normalTextureId;
    unsigned int albedoSpecTextureId;
    unsigned int depthRenderBufferId;
};
int main() {
    InitWindow(1920, 1080, "3D pixel art");
    //Shader stuff
    Shader gbufferShader=LoadShader("src/resources/shaders/gbuffer.vs","src/resources/shaders/gbuffer.fs");
    Shader deferredShader=LoadShader("src/resources/shaders/deferred_shading.vs","src/resources/shaders/deferred_shading.fs");
    deferredShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(deferredShader, "viewPosition");
    
    
    //Camera stuff
    Camera3D cam;
    cam.position={0.0,2.0,-4.0};
    cam.target={0.0,0.0,0.0};
    cam.fovy=45.0f;
    cam.up={0.0,1.0,0.0};
    cam.projection=CAMERA_PERSPECTIVE;
    Model cube=LoadModelFromMesh(GenMeshCube(1.0f,1.0f,1.0f));
    cube.materials[0].shader=gbufferShader;
    
    GBuffer gBuffer;
    gBuffer.frameBufferId=rlLoadFramebuffer();
    rlEnableFramebuffer(gBuffer.frameBufferId);

    gBuffer.positionTextureId=rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16,1);
    gBuffer.normalTextureId=rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16,1);
    gBuffer.albedoSpecTextureId=rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,1);

    rlActiveDrawBuffers(3);

    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.positionTextureId,RL_ATTACHMENT_COLOR_CHANNEL0,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.normalTextureId,RL_ATTACHMENT_COLOR_CHANNEL1,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.albedoSpecTextureId,RL_ATTACHMENT_COLOR_CHANNEL2,RL_ATTACHMENT_TEXTURE2D,0);
    
    gBuffer.depthRenderBufferId=rlLoadTextureDepth(downWidth,downHeight,true);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.depthRenderBufferId,RL_ATTACHMENT_DEPTH,RL_ATTACHMENT_RENDERBUFFER,0);
    
    
    if(rlFramebufferComplete(gBuffer.frameBufferId)){
        std::cout<<"FrameBuffer Completed Successfully"<<std::endl;
    }
    
    
    rlEnableShader(deferredShader.id);
    int texUnitPosition = 0;
    int texUnitNormal = 1;
    int texUnitAlbedoSpec = 2;
    SetShaderValue(deferredShader, rlGetLocationUniform(deferredShader.id, "gPosition"), &texUnitPosition, RL_SHADER_UNIFORM_SAMPLER2D);
    SetShaderValue(deferredShader, rlGetLocationUniform(deferredShader.id, "gNormal"), &texUnitNormal, RL_SHADER_UNIFORM_SAMPLER2D);
    SetShaderValue(deferredShader, rlGetLocationUniform(deferredShader.id, "gAlbedoSpec"), &texUnitAlbedoSpec, RL_SHADER_UNIFORM_SAMPLER2D);
    rlDisableShader();
    
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, -2 }, Vector3Zero(), YELLOW, deferredShader);
    lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 2, 1, 2 }, Vector3Zero(), RED, deferredShader);
    lights[2] = CreateLight(LIGHT_POINT, (Vector3){ -2, 1, 2 }, Vector3Zero(), GREEN, deferredShader);
    lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 2, 1, -2 }, Vector3Zero(), BLUE, deferredShader);

    ToggleFullscreen();
    rlEnableDepthTest();
    while (!WindowShouldClose()) {
       SetShaderValue(deferredShader,deferredShader.locs[SHADER_LOC_VECTOR_VIEW],&cam.position,SHADER_UNIFORM_VEC3);
       UpdateCamera(&cam,CAMERA_ORBITAL);


        // Check key inputs to enable/disable lights
        if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
        if (IsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
        if (IsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
        if (IsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }

           // Update light values (actually, only enable/disable them)
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(deferredShader, lights[i]);
        
       BeginDrawing();
            rlEnableFramebuffer(gBuffer.frameBufferId);
            rlViewport(0,0,downWidth,downHeight);
            rlClearColor(0, 0, 0, 0);
            rlClearScreenBuffers();  // Clear color and depth buffer
            rlDisableColorBlend();
            BeginMode3D(cam);
                rlEnableShader(gbufferShader.id);
                    DrawModel(cube,Vector3Zero(),0.25,WHITE);
                rlDisableShader();
            EndMode3D();
            rlEnableColorBlend();
            rlDisableFramebuffer();
            rlClearScreenBuffers();
            rlViewport(0,0,screenWidth,screenHeight);
                rlDisableColorBlend();
                rlEnableShader(deferredShader.id);
                     // Bind our g-buffer textures
                    // We are binding them to locations that we earlier set in sampler2D uniforms `gPosition`, `gNormal`,
                    // and `gAlbedoSpec`
                    rlActiveTextureSlot(texUnitPosition);
                    rlEnableTexture(gBuffer.positionTextureId);
                    rlActiveTextureSlot(texUnitNormal);
                    rlEnableTexture(gBuffer.normalTextureId);
                    rlActiveTextureSlot(texUnitAlbedoSpec);
                    rlEnableTexture(gBuffer.albedoSpecTextureId);

                    // Finally, we draw a fullscreen quad to our default framebufferId
                    // This will now be shaded using our deferred shader
                    rlLoadDrawQuad();
                    rlDisableShader();
                    rlEnableColorBlend();

                    // As a last step, we now copy over the depth buffer from our g-buffer to the default framebufferId
            rlBindFramebuffer(RL_READ_FRAMEBUFFER, gBuffer.frameBufferId);
            rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
            rlBlitFramebuffer(0, 0, downWidth, downHeight, 0, 0, screenWidth, screenHeight, 0x00000100); // GL_DEPTH_BUFFER_BIT
            rlDisableFramebuffer();
            // Since our shader is now done and disabled, we can draw spheres
            // that represent light positions in default forward rendering
            /*BeginMode3D(cam);
                rlEnableShader(rlGetShaderIdDefault());
                    for (int i = 0; i < MAX_LIGHTS; i++)
                        {
                            if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                            else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
                        }
                rlDisableShader();
            EndMode3D();*/

       EndDrawing();
    }
    
    CloseWindow();
}