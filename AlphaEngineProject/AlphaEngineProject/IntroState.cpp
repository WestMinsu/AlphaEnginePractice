#include "IntroState.h"
#include "GameManager.h"
#include "Constants.h"

void IntroState::Enter(GameManager* gameManager)
{
	gameManager->m_pTex = AEGfxTextureLoad("Assets/Digipen_logo.png");
}

void IntroState::Update(GameManager* gameManager, f32 dt)
{
}

void IntroState::Draw(GameManager* gameManager)
{
	gameManager->DrawRect(0, 0, kWindowWidth, kWindowHeight, 1.0f, 1.0f, 1.0f, 1.0f, gameManager->m_pTex);
}

void IntroState::Exit(GameManager* gameManager)
{
}
