#pragma once
#include "IGameState.h"
#include "AEEngine.h"
#include <random>
#include "Utility.h"
#include "GameManager.h"
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
    f32 m_elapsedTime;

    AELineSegment2 m_topWall;
    AELineSegment2 m_bottomWall;
    AELineSegment2 m_leftWall;
    AELineSegment2 m_rightWall;

    void SetBallAndPaddles(GameManager* gameManager);

    struct CollisionResult
    {
        bool     collided = false;
        AEVec2   normal = { 0.0f, 0.0f };
        f32      penetrationDepth = 0.0f;
        PaddleHitType hitType = PaddleHitType::kNone;
        f32      paddleCenterY = 0.0f;
    };

    CollisionResult CheckPaddleAABBCollision(
        const AEVec2& ballPos,
        f32 ballRadius,
        const AEVec2& paddlePos,
        f32 paddleWidth,
        f32 paddleHeight,
        PaddleHitType type
    );
};