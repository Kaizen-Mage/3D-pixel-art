#include "camera/camera.h"
RenderCamera::RenderCamera(){

}
RenderCamera::RenderCamera(Vector3 position,Vector3 target,Vector3 up,CameraProjection projection):
position(position),
target(target),
up(up),
projection(projection)
{
    cam.position=position;
    cam.target=target;
    cam.up=up;
    cam.fovy=fovy;  
    cam.projection=projection;
}
void RenderCamera::Update(){
    cam.position=position;   
    cam.target=target;
    cam.up=up;
    cam.fovy=fovy;  
    cam.projection=projection;
}
void RenderCamera::Follow(Vector3 followTarget){
   Vector2 directionHorizontal={cos(yaw*DEG2RAD),sin(yaw*DEG2RAD)}; 
   float pitchHeight=sin(pitch*DEG2RAD); 
   //first focus on horizontal movement so just use distance.y 
   Vector3 follow={directionHorizontal.x*orbitRadius,pitchHeight*orbitRadius,directionHorizontal.y*orbitRadius}; 
   position=Vector3Subtract(followTarget,follow); 
}
void RenderCamera::FreeLook(float moveSpeed,float dt){
    //first get the direction to where you are looking at and u it as a forward vector;
    Vector3 viewForward;
    Vector3 right;
    //Get Mouse delta to update the target
    Vector2 delta=GetMouseDelta();
    yaw+=delta.x*sensitivity;
    pitch-=delta.y*sensitivity;
    if (yaw > 360||yaw< -360) yaw = 0;
    SetMousePosition(GetScreenWidth()/2,GetScreenHeight()/2);
    pitch=Clamp(pitch,-89.99,89.99);
    //The forward vector is just spherical coordinate projection;
    viewForward={(float)(cos(yaw*DEG2RAD)*cos(pitch*DEG2RAD)),(float)sin(pitch*DEG2RAD),(float)(sin(yaw*DEG2RAD)*cos(pitch*DEG2RAD))};
    //Calculate right vector using World up and cross it with forward vector
    right=Vector3CrossProduct(viewForward,up);
    right=Vector3Normalize(right);
    //Move the target to look forwards
     //Move towards where you are looking with W
    if(IsKeyDown(KEY_W)){
        position=Vector3Add(position,Vector3Scale(viewForward,moveSpeed*dt));
        target=position+viewForward;
   
    }
    //Opposite when moving backwards thats why the negative
    if(IsKeyDown(KEY_S)){
        position=Vector3Add(position,Vector3Scale(viewForward,-moveSpeed*dt));
        target=position+viewForward;
   
    }
    //Same for strafing
    if(IsKeyDown(KEY_A)){
        position=Vector3Add(position,Vector3Scale(Vector3Zero()-right,moveSpeed*dt));
        target=position+right;
   
    }
    if(IsKeyDown(KEY_D)){
        position=Vector3Add(position,Vector3Scale(right,moveSpeed*dt));
        target=position+viewForward;
    }
    else{
        target=position+viewForward;
    }
}
void RenderCamera::Orbit(float moveSpeed,float dt){
    //first get the direction to where you are looking at and u it as a forward vector;
    Vector3 viewForward;
    //Get Mouse delta to update the target
    Vector2 delta=GetMouseDelta();
    yaw+=delta.x*sensitivity;
    pitch-=delta.y*sensitivity;
    if (yaw > 360||yaw< -360) yaw = 0;
    SetMousePosition(GetScreenWidth()/2,GetScreenHeight()/2);
    pitch=Clamp(pitch,-89.99,89.99);
    //The forward vector is just spherical coordinate projection;
    viewForward={(float)(cos(yaw*DEG2RAD)*cos(pitch*DEG2RAD)),(float)sin(pitch*DEG2RAD),(float)(sin(yaw*DEG2RAD)*cos(pitch*DEG2RAD))};
    position=target-Vector3Scale(viewForward,orbitRadius);
}
void RenderCamera::LookAt(Vector3 targetPosition){
    target=targetPosition;
}
void RenderCamera::Begin(){
    BeginMode3D(cam);
}
void RenderCamera::End(){
    EndMode3D();
}