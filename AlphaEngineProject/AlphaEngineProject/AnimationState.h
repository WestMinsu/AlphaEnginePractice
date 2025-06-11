#pragma once
#include "IGameState.h"
#include "AEEngine.h"

enum class CharacterAnimationState
{
    IDLE,
    WALK,
    JUMP,
    DEATH
};

enum class CharacterDirection
{
    LEFT,
    RIGHT
};


class AnimationState : public IGameState
{
public:
    void Enter(GameManager* gameManager) override;
    void Update(GameManager* gameManager, f32 dt) override;
    void Draw(GameManager* gameManager) override;
    void Exit(GameManager* gameManager) override;

private:
    AEGfxVertexList* m_mesh;

    AEGfxTexture* m_pTexIdle;
    AEGfxTexture* m_pTexWalk;
    AEGfxTexture* m_pTexJump;
    AEGfxTexture* m_pTexDeath;

    CharacterAnimationState m_currentAnimState; 
    CharacterDirection m_currentDirection;

    s32 m_subImageIndex;
    f32 m_offset;        
    f32 m_elapsedTime;   

    s32 m_idleFrames = 8;  
    s32 m_walkFrames = 8;  
    s32 m_jumpFrames = 8; 
    s32 m_deathFrames = 8; 

    bool m_animationFinished = false;
    f32 m_deathTimer;    
    const f32 m_restartDelay = 3.0f; 

    f32 characterWidth = 500.0f;
    f32 characterHeight = 500.0f;
    AEVec2 m_characterPosition;
    f32 m_characterSpeed = 300.0f;
};


