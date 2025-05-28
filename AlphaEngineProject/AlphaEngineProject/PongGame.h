#pragma once
#include <crtdbg.h>
#include "AEEngine.h"
#include <random> 
#include <chrono>       
#include <cmath> 

enum class PaddleHitType
{
    kNone,
    kPlayer1,
    kPlayer2,
};

class PongGame
{
public:

    PongGame();
   
    ~PongGame();

    void Initialize();

    void Update(f32 dt);

    void Draw();

    void DrawRect(f32 x, f32 y, f32 w, f32 h, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, AEGfxTexture* pTex = nullptr);

private:
    f64 m_elapsedTime;       
    bool m_isGameRunning;         
    s8 m_font;     
    AEGfxVertexList* m_mesh;
    AEGfxTexture* m_pTex;

    AEVec2 m_player1Position;
    AEVec2 m_player2Position;
    AEVec2 m_ballPosition;
    AEVec2 m_ballVelocity;
    f32 m_ballSpeed;

    // for generate random number
    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<f32> m_velocityDist;

    AELineSegment2 m_topWall;
    AELineSegment2 m_bottomWall;
    AELineSegment2 m_leftWall;
    AELineSegment2 m_rightWall;

    AELineSegment2 m_player1PaddleLeftEdge;
    AELineSegment2 m_player1PaddleRightEdge;
    AELineSegment2 m_player1PaddleTopEdge;
    AELineSegment2 m_player1PaddleBottomEdge;

    AELineSegment2 m_player2PaddleLeftEdge;
    AELineSegment2 m_player2PaddleRightEdge;
    AELineSegment2 m_player2PaddleTopEdge;
    AELineSegment2 m_player2PaddleBottomEdge;

    int m_player1Score;
    int m_player2Score;

    bool m_ballHitPaddleInitial;
    GameState m_gameState;
};