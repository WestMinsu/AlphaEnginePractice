#pragma once
#include "AEEngine.h"

class GameManager;

class IGameState 
{
public:
    virtual ~IGameState() = default; 
    virtual void Enter(GameManager* gameManager) = 0;
    virtual void Update(GameManager* gameManager, f32 dt) = 0;
    virtual void Draw(GameManager* gameManager) = 0;
    virtual void Exit(GameManager* gameManager) = 0;
};