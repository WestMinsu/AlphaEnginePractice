#pragma once
#include "IGameState.h"
#include "AEEngine.h"

class AnimationState : public IGameState
{
public:
    void Enter(GameManager* gameManager) override;
    void Update(GameManager* gameManager, f32 dt) override;
    void Draw(GameManager* gameManager) override;
    void Exit(GameManager* gameManager) override;

private:
    AEGfxVertexList* m_mesh;
    f32 m_elapsedTime;
    int m_subImageIndex;
    f32 m_offset;
};