#include "PongGame.h"
#include "AEUtil.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Constants.h"

PongGame::PongGame()
	: m_elapsedTime(0.0),
	m_gameState(GameState::MAIN_MENU),
	m_font(kInvalidFontHandle),
	m_randomEngine(std::random_device{}()),
	m_velocityDist(-1.0f, 1.0f)
{
	m_font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	AEGfxMeshStart();
	AEGfxVertexAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
	AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
	AEGfxVertexAdd(0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
	AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	m_mesh = AEGfxMeshEnd();

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

	m_pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");

	Initialize();
}

PongGame::~PongGame()
{
	if (m_font != kInvalidFontHandle)
	{
		AEGfxDestroyFont(m_font);
		m_font = kInvalidFontHandle;
	}
	AEGfxMeshFree(m_mesh);
	AEGfxTextureUnload(m_pTex);
}

void PongGame::Initialize()
{
	m_player1Position = { -kGameWindowWidth / 3.0f, 0.f };
	m_player2Position = { kGameWindowWidth / 3.0f, 0.f };
	m_ballPosition = { 0.f, 0.f };

	m_player1Score = 0;
	m_player2Score = 0;

	m_elapsedTime = 0.0;
	m_gameState = GameState::MAIN_MENU;

	m_ballSpeed = 700.0f;
	f32 randomX = m_velocityDist(m_randomEngine);
	f32 randomY = m_velocityDist(m_randomEngine);
	m_ballVelocity.x = m_velocityDist(m_randomEngine);
	m_ballVelocity.y = m_velocityDist(m_randomEngine);
	AEVec2Normalize(&m_ballVelocity, &m_ballVelocity);
	m_ballVelocity.x *= m_ballSpeed;
	m_ballVelocity.y *= m_ballSpeed;
}

void PongGame::Update(f32 dt)
{
	if (AEInputCheckTriggered(AEVK_R))
	{
		Initialize();
	}

	float p1LeftX = m_player1Position.x - kPaddleWidth / 2.0f;
	float p1RightX = m_player1Position.x + kPaddleWidth / 2.0f;
	float p1TopY = m_player1Position.y + kPaddleHeight / 2.0f;
	float p1BottomY = m_player1Position.y - kPaddleHeight / 2.0f;

	m_player1PaddleLeftEdge.mP0 = { p1LeftX, p1BottomY };
	m_player1PaddleLeftEdge.mP1 = { p1LeftX, p1TopY };
	m_player1PaddleLeftEdge.mN = { -1.0f, 0.0f };
	m_player1PaddleLeftEdge.mNdotP0 = AEVec2DotProduct(&m_player1PaddleLeftEdge.mN, &m_player1PaddleLeftEdge.mP0);

	m_player1PaddleRightEdge.mP0 = { p1RightX, p1BottomY };
	m_player1PaddleRightEdge.mP1 = { p1RightX, p1TopY };
	m_player1PaddleRightEdge.mN = { 1.0f, 0.0f };
	m_player1PaddleRightEdge.mNdotP0 = AEVec2DotProduct(&m_player1PaddleRightEdge.mN, &m_player1PaddleRightEdge.mP0);

	m_player1PaddleTopEdge.mP0 = { p1LeftX, p1TopY };
	m_player1PaddleTopEdge.mP1 = { p1RightX, p1TopY };
	m_player1PaddleTopEdge.mN = { 0.0f, 1.0f };
	m_player1PaddleTopEdge.mNdotP0 = AEVec2DotProduct(&m_player1PaddleTopEdge.mN, &m_player1PaddleTopEdge.mP0);

	m_player1PaddleBottomEdge.mP0 = { p1LeftX, p1BottomY };
	m_player1PaddleBottomEdge.mP1 = { p1RightX, p1BottomY };
	m_player1PaddleBottomEdge.mN = { 0.0f, -1.0f };
	m_player1PaddleBottomEdge.mNdotP0 = AEVec2DotProduct(&m_player1PaddleBottomEdge.mN, &m_player1PaddleBottomEdge.mP0);

	float p2LeftX = m_player2Position.x - kPaddleWidth / 2.0f;
	float p2RightX = m_player2Position.x + kPaddleWidth / 2.0f;
	float p2TopY = m_player2Position.y + kPaddleHeight / 2.0f;
	float p2BottomY = m_player2Position.y - kPaddleHeight / 2.0f;

	m_player2PaddleLeftEdge.mP0 = { p2LeftX, p2BottomY };
	m_player2PaddleLeftEdge.mP1 = { p2LeftX, p2TopY };
	m_player2PaddleLeftEdge.mN = { -1.0f, 0.0f };
	m_player2PaddleLeftEdge.mNdotP0 = AEVec2DotProduct(&m_player2PaddleLeftEdge.mN, &m_player2PaddleLeftEdge.mP0);

	m_player2PaddleRightEdge.mP0 = { p2RightX, p2BottomY };
	m_player2PaddleRightEdge.mP1 = { p2RightX, p2TopY };
	m_player2PaddleRightEdge.mN = { 1.0f, 0.0f };
	m_player2PaddleRightEdge.mNdotP0 = AEVec2DotProduct(&m_player2PaddleRightEdge.mN, &m_player2PaddleRightEdge.mP0);

	m_player2PaddleTopEdge.mP0 = { p2LeftX, p2TopY };
	m_player2PaddleTopEdge.mP1 = { p2RightX, p2TopY };
	m_player2PaddleTopEdge.mN = { 0.0f, 1.0f };
	m_player2PaddleTopEdge.mNdotP0 = AEVec2DotProduct(&m_player2PaddleTopEdge.mN, &m_player2PaddleTopEdge.mP0);

	m_player2PaddleBottomEdge.mP0 = { p2LeftX, p2BottomY };
	m_player2PaddleBottomEdge.mP1 = { p2RightX, p2BottomY };
	m_player2PaddleBottomEdge.mN = { 0.0f, -1.0f };
	m_player2PaddleBottomEdge.mNdotP0 = AEVec2DotProduct(&m_player2PaddleBottomEdge.mN, &m_player2PaddleBottomEdge.mP0);

	switch (m_gameState)
	{
	case GameState::MAIN_MENU:
	{
		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			m_gameState = GameState::MAIN_GAME;
		}
		break;
	case GameState::MAIN_GAME:
	{
		s32 cursorX, cursorY;
		AEInputGetCursorPosition(&cursorX, &cursorY);
		std::cout << cursorY << std::endl;

		if ((m_player1Position.y + (cursorY - kHalfWindowHeight)) < -5) // Coordinate Transformation e.g.) 0 -> 450, 900 -> -450
		{
			m_player1Position.y += kPaddleMoveSpeed * dt;
		}
		else if ((m_player1Position.y + (cursorY - kHalfWindowHeight)) > 5)
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

		// wall collision
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
						m_gameState = GameState::RESULT;
						m_winMessage = "PLAYER A WINS!";
					}
				}
				else if (wall == &m_leftWall)
				{
					m_player2Score++;
					if (m_player2Score >= kWinScore)
					{
						m_gameState = GameState::RESULT;
						m_winMessage = "PLAYER B WINS!";
					}
				}
			}
		}

		// corner collision
		AEVec2 p1Corners[] =
		{
			{m_player1Position.x - kPaddleWidth / 2.0f, m_player1Position.y - kPaddleHeight / 2.0f}, // Bottom-Left
			{m_player1Position.x + kPaddleWidth / 2.0f, m_player1Position.y - kPaddleHeight / 2.0f}, // Bottom-Right
			{m_player1Position.x - kPaddleWidth / 2.0f, m_player1Position.y + kPaddleHeight / 2.0f}, // Top-Left
			{m_player1Position.x + kPaddleWidth / 2.0f, m_player1Position.y + kPaddleHeight / 2.0f}  // Top-Right
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

		AELineSegment2* p1Paddles[] = {
			&m_player1PaddleLeftEdge, &m_player1PaddleRightEdge,
			&m_player1PaddleTopEdge, &m_player1PaddleBottomEdge
		};
		for (AELineSegment2* paddleEdge : p1Paddles)
		{
			collisionTime = AEAnimatedCircleToStaticLineSegment(&currentBallPos, &nextBallPos, kBallRadius, paddleEdge, &intersectionPoint);
			if (collisionTime >= 0.0f && collisionTime < minCollisionTime)
			{
				minCollisionTime = collisionTime;
				bestCollisionNormal = paddleEdge->mN;
				collisionOccurred = true;
				paddleHitType = PaddleHitType::kPlayer1;
				paddleCenterY = m_player1Position.y;
			}
		}

		AELineSegment2* p2Paddles[] = {
			&m_player2PaddleLeftEdge, &m_player2PaddleRightEdge,
			&m_player2PaddleTopEdge, &m_player2PaddleBottomEdge
		};
		for (AELineSegment2* paddleEdge : p2Paddles)
		{
			collisionTime = AEAnimatedCircleToStaticLineSegment(&currentBallPos, &nextBallPos, kBallRadius, paddleEdge, &intersectionPoint);
			if (collisionTime >= 0.0f && collisionTime < minCollisionTime)
			{
				minCollisionTime = collisionTime;
				bestCollisionNormal = paddleEdge->mN;
				collisionOccurred = true;
				paddleHitType = PaddleHitType::kPlayer2;
				paddleCenterY = m_player2Position.y;
			}
		}

		if (collisionOccurred)
		{
			f32 dotProduct = AEVec2DotProduct(&m_ballVelocity, &bestCollisionNormal);
			m_ballVelocity.x -= 2.0f * dotProduct * bestCollisionNormal.x;
			m_ballVelocity.y -= 2.0f * dotProduct * bestCollisionNormal.y;

			if (paddleHitType != PaddleHitType::kNone)
			{
				AEVec2 finalCollisionBallPos = {
					currentBallPos.x + m_ballVelocity.x * dt * minCollisionTime,
					currentBallPos.y + m_ballVelocity.y * dt * minCollisionTime
				};

				float hitPointY = finalCollisionBallPos.y - paddleCenterY;
				float normalizedHitPointY = hitPointY / (kPaddleHeight / 2.0f);
				m_ballVelocity.y += normalizedHitPointY * m_ballSpeed * 0.5f;
			}

			AEVec2Normalize(&m_ballVelocity, &m_ballVelocity);
			m_ballVelocity.x *= m_ballSpeed;
			m_ballVelocity.y *= m_ballSpeed;

			m_ballPosition.x = currentBallPos.x + m_ballVelocity.x * dt * minCollisionTime;
			m_ballPosition.y = currentBallPos.y + m_ballVelocity.y * dt * minCollisionTime;
		}
		else
		{
			m_ballPosition = nextBallPos;
		}
	}
	}
	}
}
void PongGame::Draw()
{
	AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

	switch (m_gameState)
	{
	case GameState::MAIN_MENU:
	{
		std::string displayText = "Press SPACE key to start...";
		f32 TextScale = 1.2f;

		f32 w, h;
		AEGfxGetPrintSize(m_font, displayText.c_str(), TextScale, &w, &h);

		f32 textXPosition = -w / 2;
		f32 textYPosition = -h / 2;

		AEGfxPrint(m_font, displayText.c_str(), textXPosition, textYPosition, TextScale, 1, 1, 1, 1);
		break;
	}
	case GameState::MAIN_GAME:
	{
		// Game Scene
		DrawRect(m_player1Position.x, m_player1Position.y, kPaddleWidth, kPaddleHeight, 1.0f, 1.0f, 0.0f, 1.0f);
		DrawRect(m_player2Position.x, m_player2Position.y, kPaddleWidth, kPaddleHeight, 0.0f, 1.0f, 1.0f, 1.0f);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(m_pTex, 0, 0);
		DrawRect(m_ballPosition.x, m_ballPosition.y, kBallRadius * 2.0f, kBallRadius * 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, m_pTex);


		// HUD
		std::stringstream ssTime;
		ssTime << "Time: " << std::fixed << std::setprecision(1) << m_elapsedTime << "s";

		f32 TextScale = 0.8f;
		f32 w, h;
		AEGfxGetPrintSize(m_font, ssTime.str().c_str(), TextScale, &w, &h);

		f32 time_x = -w / 2;
		f32 time_y = 0.9f - h;
		AEGfxPrint(m_font, ssTime.str().c_str(), time_x, time_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);

		std::stringstream ssP1;
		ssP1 << m_player1Score;

		f32 p1_x = -2.0f / 3.0f;
		f32 p1_y = time_y;
		AEGfxPrint(m_font, ssP1.str().c_str(), p1_x, p1_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);

		std::stringstream ssP2;
		ssP2 << m_player2Score;
		f32 p2_x = -p1_x;
		f32 p2_y = p1_y;
		AEGfxPrint(m_font, ssP2.str().c_str(), p2_x, p2_y, TextScale, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
	case GameState::RESULT:
	{
		f32 win_w, win_h;
		AEGfxGetPrintSize(m_font, m_winMessage.c_str(), 1.5f, &win_w, &win_h);
		f32 win_x = -win_w / 2.0f;
		f32 win_y = 0.1f;

		if (m_winMessage.find("PLAYER A") != std::string::npos) {
			AEGfxPrint(m_font, m_winMessage.c_str(), win_x, win_y, 1.5f, 1.0f, 1.0f, 0.0f, 1.0f); 
		}
		else {
			AEGfxPrint(m_font, m_winMessage.c_str(), win_x, win_y, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f); 
		}

		std::string restartMsg = "Press R to Restart";
		f32 restart_w, restart_h;
		AEGfxGetPrintSize(m_font, restartMsg.c_str(), 1.0f, &restart_w, &restart_h);
		f32 restart_x = -restart_w / 2.0f;
		f32 restart_y = -0.1f; // 승리 메시지 아래

		AEGfxPrint(m_font, restartMsg.c_str(), restart_x, restart_y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); 
	}
	break;
	}
}

void PongGame::DrawRect(f32 x, f32 y, f32 w, f32 h, float r, float g, float b, float a, AEGfxTexture* pTex)
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
