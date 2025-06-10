#include "MainGameState.h"
#include "GameManager.h"
#include "Constants.h"
#include "AEUtil.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

void MainGameState::Enter(GameManager* gameManager)
{
    m_randomEngine.seed(std::random_device{}());
    m_velocityDist = std::uniform_real_distribution<f32>(-1.0f, 1.0f);

    m_topWall.mP0 = { -kHalfWindowWidth, kHalfWindowHeight };
    m_topWall.mP1 = { kHalfWindowWidth, kHalfWindowHeight };
    m_topWall.mN = { 0.0f, -1.0f };
    m_topWall.mNdotP0 = AEVec2DotProduct(&m_topWall.mN, &m_topWall.mP0);

    m_bottomWall.mP0 = { -kHalfWindowWidth, -kHalfWindowHeight };
    m_bottomWall.mP1 = { kHalfWindowWidth, -kHalfWindowHeight };
    m_bottomWall.mN = { 0.0f, 1.0f };
    m_bottomWall.mNdotP0 = AEVec2DotProduct(&m_bottomWall.mN, &m_bottomWall.mP0);

    m_leftWall.mP0 = { -kHalfWindowWidth, -kHalfWindowHeight };
    m_leftWall.mP1 = { -kHalfWindowWidth, kHalfWindowHeight };
    m_leftWall.mN = { 1.0f, 0.0f };
    m_leftWall.mNdotP0 = AEVec2DotProduct(&m_leftWall.mN, &m_leftWall.mP0);

    m_rightWall.mP0 = { kHalfWindowWidth, -kHalfWindowHeight };
    m_rightWall.mP1 = { kHalfWindowWidth, kHalfWindowHeight };
    m_rightWall.mN = { -1.0f, 0.0f };
    m_rightWall.mNdotP0 = AEVec2DotProduct(&m_rightWall.mN, &m_rightWall.mP0);
    m_ballSpeed = 700.0f;

    SetBallAndPaddles(gameManager);
    m_player1Score = 0;
    m_player2Score = 0;
    m_elapsedTime = 0.0;
    gameManager->m_pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void MainGameState::Update(GameManager* gameManager, f32 dt)
{
    if (AEInputCheckTriggered(AEVK_R))
    {
        gameManager->ChangeState(GameState::MAIN_MENU);
        return;
    }

    s32 cursorX, cursorY;
    AEInputGetCursorPosition(&cursorX, &cursorY);

    if ((m_player1Position.y + (cursorY - kHalfWindowHeight)) < -5.0f)
    {
        m_player1Position.y += kPaddleMoveSpeed * dt;
    }
    else if ((m_player1Position.y + (cursorY - kHalfWindowHeight)) > 5.0f)
    {
        m_player1Position.y -= kPaddleMoveSpeed * dt;
    }

    m_player1Position.y = AEClamp(m_player1Position.y, -(kHalfWindowHeight - kPaddleHeight / 2.0f), kHalfWindowHeight - kPaddleHeight / 2.0f);

    if (AEInputCheckCurr(AEVK_W))
    {
        m_player2Position.y += kPaddleMoveSpeed * dt;
    }
    if (AEInputCheckCurr(AEVK_S))
    {
        m_player2Position.y -= kPaddleMoveSpeed * dt;
    }

    m_player2Position.y = AEClamp(m_player2Position.y, -(kHalfWindowHeight - kPaddleHeight / 2.0f), kHalfWindowHeight - kPaddleHeight / 2.0f);

    m_elapsedTime += dt;
    AEVec2 currentBallPos = m_ballPosition;
    AEVec2 nextBallPos = { m_ballPosition.x + m_ballVelocity.x * dt,
                            m_ballPosition.y + m_ballVelocity.y * dt };

    f32 minCollisionTime = 1.0f;
    AEVec2 bestCollisionNormal = { 0.0f, 0.0f };
    bool collisionOccurred = false;

    PaddleHitType paddleHitType = PaddleHitType::kNone;
    f32 paddleCenterY = 0.0f;

    AEVec2 intersectionPoint;
    f32 collisionTime;

    AELineSegment2* walls[] = { &m_topWall, &m_bottomWall, &m_leftWall, &m_rightWall };
    for (AELineSegment2* wall : walls)
    {
        collisionTime = AEAnimatedCircleToStaticLineSegment(&currentBallPos, &nextBallPos, kBallRadius, wall, &intersectionPoint);
        if (collisionTime >= 0.0f && collisionTime < minCollisionTime)
        {
            minCollisionTime = collisionTime;
            bestCollisionNormal = wall->mN;
            collisionOccurred = true;
            paddleHitType = PaddleHitType::kNone;

            if (wall == &m_rightWall)
            {
                m_player1Score++;
                if (m_player1Score >= kWinScore)
                {
                    gameManager->SetNextResultStateMessage("PLAYER A WINS!");
                    gameManager->ChangeState(GameState::RESULT);
                }
            }
            else if (wall == &m_leftWall)
            {
                m_player2Score++;
                if (m_player2Score >= kWinScore)
                {
                    gameManager->SetNextResultStateMessage("PLAYER B WINS!");
                    gameManager->ChangeState(GameState::RESULT);
                }
            }
        }
    }

    AEVec2 p1Corners[] =
    {
        {m_player1Position.x - kPaddleWidth / 2.0f, m_player1Position.y - kPaddleHeight / 2.0f},
        {m_player1Position.x + kPaddleWidth / 2.0f, m_player1Position.y - kPaddleHeight / 2.0f},
        {m_player1Position.x - kPaddleWidth / 2.0f, m_player1Position.y + kPaddleHeight / 2.0f},
        {m_player1Position.x + kPaddleWidth / 2.0f, m_player1Position.y + kPaddleHeight / 2.0f}
    };
    for (AEVec2& corner : p1Corners)
    {
        collisionTime = AEAnimatedPointToStaticCircle(&currentBallPos, &nextBallPos, &corner, kBallRadius, &intersectionPoint);

        if (collisionTime >= 0.0f && collisionTime < minCollisionTime)
        {
            minCollisionTime = collisionTime;

            AEVec2 ballCollisionPos = {
                currentBallPos.x + m_ballVelocity.x * dt * minCollisionTime,
                currentBallPos.y + m_ballVelocity.y * dt * minCollisionTime
            };
            AEVec2 tempNormal;
            AEVec2Sub(&tempNormal, &ballCollisionPos, &corner);
            AEVec2Normalize(&bestCollisionNormal, &tempNormal);

            collisionOccurred = true;
            paddleHitType = PaddleHitType::kPlayer1;
            paddleCenterY = m_player1Position.y;
        }
    }

    AEVec2 p2Corners[] = {
        {m_player2Position.x - kPaddleWidth / 2.0f, m_player2Position.y - kPaddleHeight / 2.0f},
        {m_player2Position.x + kPaddleWidth / 2.0f, m_player2Position.y - kPaddleHeight / 2.0f},
        {m_player2Position.x - kPaddleWidth / 2.0f, m_player2Position.y + kPaddleHeight / 2.0f},
        {m_player2Position.x + kPaddleWidth / 2.0f, m_player2Position.y + kPaddleHeight / 2.0f}
    };
    for (const AEVec2& corner : p2Corners)
    {
        collisionTime = AEAnimatedPointToStaticCircle(&currentBallPos, &nextBallPos, (AEVec2*)&corner, kBallRadius, &intersectionPoint);

        if (collisionTime >= 0.0f && collisionTime < minCollisionTime)
        {
            minCollisionTime = collisionTime;

            AEVec2 ballCollisionPos = {
                currentBallPos.x + m_ballVelocity.x * dt * minCollisionTime,
                currentBallPos.y + m_ballVelocity.y * dt * minCollisionTime
            };
            AEVec2 tempNormal;
            AEVec2Sub(&tempNormal, &ballCollisionPos, (AEVec2*)&corner);
            AEVec2Normalize(&bestCollisionNormal, &tempNormal);

            collisionOccurred = true;
            paddleHitType = PaddleHitType::kPlayer2;
            paddleCenterY = m_player2Position.y;
        }
    }

    f32 ballAABB_MinX = m_ballPosition.x - kBallRadius;
    f32 ballAABB_MaxX = m_ballPosition.x + kBallRadius;
    f32 ballAABB_MinY = m_ballPosition.y - kBallRadius;
    f32 ballAABB_MaxY = m_ballPosition.y + kBallRadius;

    f32 p1PaddleAABB_MinX = m_player1Position.x - kPaddleWidth / 2.0f;
    f32 p1PaddleAABB_MaxX = m_player1Position.x + kPaddleWidth / 2.0f;
    f32 p1PaddleAABB_MinY = m_player1Position.y - kPaddleHeight / 2.0f;
    f32 p1PaddleAABB_MaxY = m_player1Position.y + kPaddleHeight / 2.0f;

    f32 p2PaddleAABB_MinX = m_player2Position.x - kPaddleWidth / 2.0f;
    f32 p2PaddleAABB_MaxX = m_player2Position.x + kPaddleWidth / 2.0f;
    f32 p2PaddleAABB_MinY = m_player2Position.y - kPaddleHeight / 2.0f;
    f32 p2PaddleAABB_MaxY = m_player2Position.y + kPaddleHeight / 2.0f;

    CollisionResult p1AABBCollision = CheckPaddleAABBCollision(m_ballPosition, kBallRadius,
        m_player1Position, kPaddleWidth, kPaddleHeight, PaddleHitType::kPlayer1);

    if (p1AABBCollision.collided)
    {
        m_ballPosition.x += p1AABBCollision.normal.x * p1AABBCollision.penetrationDepth;
        m_ballPosition.y += p1AABBCollision.normal.y * p1AABBCollision.penetrationDepth;

        minCollisionTime = 0.0f;
        bestCollisionNormal = p1AABBCollision.normal;
        collisionOccurred = true;
        paddleHitType = p1AABBCollision.hitType;
        paddleCenterY = p1AABBCollision.paddleCenterY;
    }

    CollisionResult p2AABBCollision = CheckPaddleAABBCollision(m_ballPosition, kBallRadius,
        m_player2Position, kPaddleWidth, kPaddleHeight, PaddleHitType::kPlayer2);

    if (p2AABBCollision.collided)
    {
        m_ballPosition.x += p2AABBCollision.normal.x * p2AABBCollision.penetrationDepth;
        m_ballPosition.y += p2AABBCollision.normal.y * p2AABBCollision.penetrationDepth;

        minCollisionTime = 0.0f;
        bestCollisionNormal = p2AABBCollision.normal;
        collisionOccurred = true;
        paddleHitType = p2AABBCollision.hitType;
        paddleCenterY = p2AABBCollision.paddleCenterY;
    }

    if (collisionOccurred)
    {
        f32 dotProduct = AEVec2DotProduct(&m_ballVelocity, &bestCollisionNormal);
        m_ballVelocity.x -= 2.0f * dotProduct * bestCollisionNormal.x;
        m_ballVelocity.y -= 2.0f * dotProduct * bestCollisionNormal.y;

        if (paddleHitType != PaddleHitType::kNone)
        {
            float hitPointY = m_ballPosition.y - paddleCenterY;
            float normalizedHitPointY = hitPointY / (kPaddleHeight / 2.0f);
            m_ballVelocity.y += normalizedHitPointY * m_ballSpeed * 0.5f;
        }

        AEVec2Normalize(&m_ballVelocity, &m_ballVelocity);
        m_ballVelocity.x *= m_ballSpeed;
        m_ballVelocity.y *= m_ballSpeed;
        if (minCollisionTime > 0.0f)
        { 
            m_ballPosition.x = currentBallPos.x + m_ballVelocity.x * dt * minCollisionTime;
            m_ballPosition.y = currentBallPos.y + m_ballVelocity.y * dt * minCollisionTime;
        }
    }
    else
    {
        m_ballPosition = nextBallPos;
    }
}

void MainGameState::Draw(GameManager* gameManager)
{
    gameManager->DrawRect(m_player1Position.x, m_player1Position.y, kPaddleWidth, kPaddleHeight, 1.0f, 1.0f, 0.0f, 1.0f);
    gameManager->DrawRect(m_player2Position.x, m_player2Position.y, kPaddleWidth, kPaddleHeight, 0.0f, 1.0f, 1.0f, 1.0f);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);
    AEGfxTextureSet(gameManager->m_pTex, 0, 0);
    gameManager->DrawRect(m_ballPosition.x, m_ballPosition.y, kBallRadius * 2.0f, kBallRadius * 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, gameManager->m_pTex);

    std::stringstream ssTime;
    ssTime << "Time: " << std::fixed << std::setprecision(1) << m_elapsedTime << "s";

    f32 TextScale = 0.8f;
    f32 w, h;
    AEGfxGetPrintSize(gameManager->m_font, ssTime.str().c_str(), TextScale, &w, &h);

    f32 time_x = -w / 2;
    f32 time_y = 0.9f - h;
    AEGfxPrint(gameManager->m_font, ssTime.str().c_str(), time_x, time_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);

    std::stringstream ssP1;
    ssP1 << m_player1Score;

    f32 p1_x = -2.0f / 3.0f;
    f32 p1_y = time_y;
    AEGfxPrint(gameManager->m_font, ssP1.str().c_str(), p1_x, p1_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);

    std::stringstream ssP2;
    ssP2 << m_player2Score;
    f32 p2_x = 2.0f / 3.0f;
    f32 p2_y = time_y;
    AEGfxPrint(gameManager->m_font, ssP2.str().c_str(), p2_x, p2_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);
}

void MainGameState::Exit(GameManager* gameManager)
{

}

void MainGameState::SetBallAndPaddles(GameManager* gameManager)
{
    m_player1Position = { -kWindowWidth / 3.0f, 0.f };
    m_player2Position = { kWindowWidth / 3.0f, 0.f };
    m_ballPosition = { 0.f, 0.f };

    f32 randomX = m_velocityDist(m_randomEngine);
    f32 randomY = m_velocityDist(m_randomEngine);
    m_ballVelocity.x = randomX;
    m_ballVelocity.y = randomY;
    AEVec2Normalize(&m_ballVelocity, &m_ballVelocity);
    m_ballVelocity.x *= m_ballSpeed;
    m_ballVelocity.y *= m_ballSpeed;
}

MainGameState::CollisionResult MainGameState::CheckPaddleAABBCollision(
    const AEVec2& ballPos,
    f32 ballRadius,
    const AEVec2& paddlePos,
    f32 paddleWidth,
    f32 paddleHeight,
    PaddleHitType type
)
{
    CollisionResult result;

    f32 ballMinX = ballPos.x - ballRadius;
    f32 ballMaxX = ballPos.x + ballRadius;
    f32 ballMinY = ballPos.y - ballRadius;
    f32 ballMaxY = ballPos.y + ballRadius;

    f32 paddleMinX = paddlePos.x - paddleWidth / 2.0f;
    f32 paddleMaxX = paddlePos.x + paddleWidth / 2.0f;
    f32 paddleMinY = paddlePos.y - paddleHeight / 2.0f;
    f32 paddleMaxY = paddlePos.y + paddleHeight / 2.0f;

    if (ballMaxX > paddleMinX && ballMinX < paddleMaxX &&
        ballMaxY > paddleMinY && ballMinY < paddleMaxY)
    {
        result.collided = true;
        result.hitType = type;
        result.paddleCenterY = paddlePos.y;

        f32 overlapLeft = ballMaxX - paddleMinX;  
        f32 overlapRight = paddleMaxX - ballMinX;
        f32 overlapBottom = ballMaxY - paddleMinY; 
        f32 overlapTop = paddleMaxY - ballMinY;   

        if (type == PaddleHitType::kPlayer1)
        {
            if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom)
            {
                result.normal = { -1.0f, 0.0f };
                result.penetrationDepth = overlapLeft;
            }
            else if (overlapRight < overlapTop && overlapRight < overlapBottom)
            {
                result.normal = { 1.0f, 0.0f }; 
                result.penetrationDepth = overlapRight;
            }
            else if (overlapBottom < overlapTop)
            {
                result.normal = { 0.0f, -1.0f };
                result.penetrationDepth = overlapBottom;
            }
            else
            {
                result.normal = { 0.0f, 1.0f };
                result.penetrationDepth = overlapTop;
            }
        }
        else if (type == PaddleHitType::kPlayer2)
        {
            if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom)
            {
                result.normal = { 1.0f, 0.0f }; 
                result.penetrationDepth = overlapRight;
            }
            else if (overlapLeft < overlapTop && overlapLeft < overlapBottom)
            {
                result.normal = { -1.0f, 0.0f };
                result.penetrationDepth = overlapLeft;
            }
            else if (overlapBottom < overlapTop)
            {
                result.normal = { 0.0f, -1.0f }; 
                result.penetrationDepth = overlapBottom;
            }
            else
            {
                result.normal = { 0.0f, 1.0f }; 
                result.penetrationDepth = overlapTop;
            }
        }
    }
    return result;
}


