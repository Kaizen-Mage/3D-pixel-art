#pragma once
#pragma once
#include "raylib.h"
#include "raymath.h"
#include "engine/engineContext.h"
class GameState{
protected:
    EngineContext* context;
public:
    //Game state variables
    GameState(EngineContext* ctx);
    virtual void Destroy();
    virtual void Update(float dt);
    virtual void Draw();
};