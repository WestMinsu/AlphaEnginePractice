#include "MainMenuState.h"
#include "GameManager.h"
#include <string>
#include "Constants.h"
#include <iostream>

const f32 buttonWidth = 350.f;
const f32 buttonHeight = 100.f;

const f32 pongButtonX = 0.f;
const f32 pongButtonY = 200.f;

const f32 animationButtonX = 0.f;
const f32 animationButtonY = -200.f;


void MainMenuState::Enter(GameManager* gameManager)
{
}

void MainMenuState::Update(GameManager* gameManager, f32 dt)
{
	s32 pX, pY;
	AEInputGetCursorPosition(&pX, &pY);
	pX -= 800;
	pY = 450 - pY;
	if (AEInputCheckReleased(AEVK_LBUTTON))
	{
		if ((pongButtonX - buttonWidth / 2 <= pX && pX <= pongButtonX + buttonWidth / 2) && (pongButtonY - buttonHeight / 2 <= pY && pY <= pongButtonY + buttonHeight / 2))
			gameManager->ChangeState(GameState::MAIN_GAME);
		if ((animationButtonX - buttonWidth / 2 <= pX && pX <= animationButtonX + buttonWidth / 2) && (animationButtonY - buttonHeight / 2 <= pY && pY <= animationButtonY + buttonHeight / 2))
			gameManager->ChangeState(GameState::ANIMATION);
	}
}

void MainMenuState::Draw(GameManager* gameManager)
{
	AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);
	gameManager->DrawRect(pongButtonX, pongButtonY, buttonWidth, buttonHeight, 0.5, 0.5, 0.5, 1);
	gameManager->DrawRect(animationButtonX, animationButtonY, buttonWidth, buttonHeight, 0.5, 0.5, 0.5, 1);

	std::string pongButtonText = "Pong";
	std::string animationButtonText = "Animation";
	f32 TextScale = 0.9f;

	f32 pongButtonTextWidth, pongButtonTextHeight;
	AEGfxGetPrintSize(gameManager->m_font, pongButtonText.c_str(), TextScale, &pongButtonTextWidth, &pongButtonTextHeight);
	f32 animationButtonTextWidth, animationButtonTextHeight;
	AEGfxGetPrintSize(gameManager->m_font, animationButtonText.c_str(), TextScale, &animationButtonTextWidth, &animationButtonTextHeight);

	f32 pongTextX = 2 * pongButtonX / kGameWindowWidth - pongButtonTextWidth / 2;
	f32 pongtextY = 2 * pongButtonY / kGameWindowHeight - pongButtonTextHeight / 2;

	f32 animationTextX = 2 * animationButtonX / kGameWindowWidth - animationButtonTextWidth / 2;
	f32 animationTextY = 2 * animationButtonY / kGameWindowHeight - animationButtonTextHeight / 2;

	AEGfxPrint(gameManager->m_font, pongButtonText.c_str(), pongTextX, pongtextY, TextScale, 1, 1, 1, 1);
	AEGfxPrint(gameManager->m_font, animationButtonText.c_str(), animationTextX, animationTextY, TextScale, 1, 1, 1, 1);
}

void MainMenuState::Exit(GameManager* gameManager)
{
}