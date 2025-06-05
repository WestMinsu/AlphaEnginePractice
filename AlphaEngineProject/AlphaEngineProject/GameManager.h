#pragma once
#include <crtdbg.h>
#include "AEEngine.h"
#include <string>
#include <random>

class IGameState; 
class MainMenuState;
class MainGameState;
class AnimationState;
class ResultState;

enum class GameState
{
    MAIN_MENU,
    MAIN_GAME,
    ANIMATION,
    RESULT,
};

enum class PaddleHitType
{
    kNone,
    kPlayer1,
    kPlayer2,
};

class GameManager
{
public:
    GameManager();
    ~GameManager();

    void Initialize();
    void Update(f32 dt);
    void Draw();

    void ChangeState(GameState newState);

    void DrawRect(f32 x, f32 y, f32 w, f32 h, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, AEGfxTexture* pTex = nullptr);

    s8 m_font;
    AEGfxTexture* m_pTex;
    std::string m_winMessage;

    AEAudio m_jumpSound;      
    AEAudioGroup m_sfxGroup;  

private:
    IGameState* m_currentState;
    MainMenuState* m_mainMenuState;
    MainGameState* m_mainGameState;
    AnimationState* m_animationState;
    ResultState* m_resultState;
    AEGfxVertexList* m_mesh;
};