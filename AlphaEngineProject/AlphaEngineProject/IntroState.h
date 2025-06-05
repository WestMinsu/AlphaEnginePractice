#pragma once
#include "IGameState.h"
#include "AEEngine.h"

class IntroState : public IGameState
{
public:
    void Enter(GameManager* gameManager) override;
    void Update(GameManager* gameManager, f32 dt) override;
    void Draw(GameManager* gameManager) override;
    void Exit(GameManager* gameManager) override;

private:
    AEGfxTexture* m_pTex;
};