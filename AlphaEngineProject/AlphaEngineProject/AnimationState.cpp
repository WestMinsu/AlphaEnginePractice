#include "AnimationState.h"
#include "GameManager.h"

void AnimationState::Enter(GameManager* gameManager)
{
    if (gameManager->m_pTex)
    {
        AEGfxTextureUnload(gameManager->m_pTex);
        gameManager->m_pTex = nullptr;
    }

    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 0.125f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 0.125f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 0.125f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    m_mesh = AEGfxMeshEnd();
    gameManager->m_pTex = AEGfxTextureLoad("Assets/idle_right_down.png");
    m_subImageIndex = 0;
}

void AnimationState::Update(GameManager* gameManager, f32 dt)
{
    if (AEInputCheckTriggered(AEVK_R))
    {
        gameManager->ChangeState(GameState::MAIN_MENU);
        return;
    }

    m_elapsedTime += dt;
    if (m_elapsedTime >= 0.1f)
    {
        m_subImageIndex = (m_subImageIndex + 1) % 8;
        m_offset = static_cast<f32>(m_subImageIndex) / 8;
        m_elapsedTime = 0;
    }
}

void AnimationState::Draw(GameManager* gameManager)
{
    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, 500, 500);

    AEMtx33 rotate = { 0 };
    AEMtx33Rot(&rotate, 0);

    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, 0, 0);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    AEGfxTextureSet(gameManager->m_pTex, m_offset, 0);

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);
    
}

void AnimationState::Exit(GameManager* gameManager)
{
    AEGfxMeshFree(m_mesh);
}

