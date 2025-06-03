#pragma once
#include "IGameState.h"
#include "AEEngine.h"
#include <random>

class MainGameState : public IGameState 
{
public:
    void Enter(GameManager* gameManager) override;
    void Update(GameManager* gameManager, f32 dt) override;
    void Draw(GameManager* gameManager) override;
    void Exit(GameManager* gameManager) override;

private:
    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<f32> m_velocityDist;

    AEVec2 m_player1Position;
    AEVec2 m_player2Position;
    AEVec2 m_ballPosition;
    AEVec2 m_ballVelocity;
    f32 m_ballSpeed;
     
    int m_player1Score;
    int m_player2Score;
    f64 m_elapsedTime;

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

    void SetBallAndPaddles(GameManager* gameManager);
    void UpdatePaddleBoundaries(GameManager* gameManager);
};