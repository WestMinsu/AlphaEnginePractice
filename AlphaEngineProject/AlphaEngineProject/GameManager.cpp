#include "GameManager.h"
#include "Constants.h"
#include "AEUtil.h"
#include "IntroState.h"
#include "MainMenuState.h"
#include "MainGameState.h"
#include "AnimationState.h"
#include "ResultState.h"
#include "IGameState.h" 

GameManager::GameManager()
    : m_font(kInvalidFontHandle),
    m_mesh(nullptr),
    m_pTex(nullptr),
    m_currentState(nullptr),
    m_introState(new IntroState()),
    m_mainMenuState(new MainMenuState()),
    m_mainGameState(new MainGameState()),
    m_animationState(new AnimationState()),
    m_resultState(new ResultState())
{
    m_font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    m_mesh = AEGfxMeshEnd();

    m_jumpSound = AEAudioLoadSound("Assets/jump.wav");
    m_sfxGroup = AEAudioCreateGroup();

    Initialize();
}

GameManager::~GameManager()
{
    if (m_font != kInvalidFontHandle)
    {
        AEGfxDestroyFont(m_font);
        m_font = kInvalidFontHandle;
    }
    AEGfxMeshFree(m_mesh);
    AEGfxTextureUnload(m_pTex);

    delete m_mainMenuState;
    delete m_mainGameState;
    delete m_resultState;
    delete m_animationState; 
}

void GameManager::Initialize()
{
    ChangeState(GameState::INTRO);
}

void GameManager::Update(f32 dt)
{
    if (m_currentState)
        m_currentState->Update(this, dt);
}

void GameManager::Draw()
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);
    if (m_currentState)
        m_currentState->Draw(this);
}

void GameManager::ChangeState(GameState newState)
{
    if (m_currentState)
        m_currentState->Exit(this);

    switch (newState)
    {
    case GameState::INTRO:
        m_currentState = m_introState;
        break;
    case GameState::MAIN_MENU:
        m_currentState = m_mainMenuState;
        break;
    case GameState::MAIN_GAME:
        m_currentState = m_mainGameState;
        break;
    case GameState::ANIMATION:
        m_currentState = m_animationState;
        break;
    case GameState::RESULT:
        m_currentState = m_resultState;
        m_resultState->SetWinMessage(m_nextResultStateMessage);
        break;
    }

    if (m_currentState)
        m_currentState->Enter(this);
}


void GameManager::DrawRect(f32 x, f32 y, f32 w, f32 h, float r, float g, float b, float a, AEGfxTexture* pTex)
{
    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, w, h);

    AEMtx33 rotate = { 0 };
    AEMtx33Rot(&rotate, 0);

    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, x, y);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &transform);

    if (pTex == nullptr)
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    else
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    AEGfxSetColorToMultiply(r, g, b, a);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    AEGfxTextureSet(pTex, 0, 0);

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);
}

void GameManager::SetNextResultStateMessage(const std::string& message)
{
    m_nextResultStateMessage = message; 
}
