#include "IntroState.h"
#include "GameManager.h"
#include "Constants.h"
#include <iostream>
void IntroState::Enter(GameManager* gameManager)
{
	gameManager->m_pTex = AEGfxTextureLoad("Assets/Digipen_logo.png");
	m_alphaTime = 0.0f;
	m_alphaSpeed = 1.0f;
	m_currentAlpha = 0.0f;
}

void IntroState::Update(GameManager* gameManager, f32 dt)
{
	if (AEInputCheckTriggered(AEVK_SPACE))
		gameManager->ChangeState(GameState::MAIN_MENU);

	m_alphaTime += dt * m_alphaSpeed;
	m_currentAlpha = (sinf(m_alphaTime));
	if (m_alphaTime >= PI)
	{
		gameManager->ChangeState(GameState::MAIN_MENU);
	}
}

void IntroState::Draw(GameManager* gameManager)
{
	gameManager->DrawRect(0, 0, kWindowWidth, kWindowHeight, 1.0f, 1.0f, 1.0f, m_currentAlpha, gameManager->m_pTex);
}

void IntroState::Exit(GameManager* gameManager)
{
}