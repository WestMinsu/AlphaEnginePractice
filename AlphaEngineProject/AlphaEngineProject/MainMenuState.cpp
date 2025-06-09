#include "MainMenuState.h"
#include "GameManager.h"
#include <string>
#include "Constants.h"
#include <iostream>
#include "Utility.h"

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
	pX -= static_cast<s32>(kHalfWindowWidth);
	pY = static_cast<s32>(kHalfWindowHeight) - pY;
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

	AEVec2 pongTextCoordinate = ConvertPixelToNDC(pongButtonX, pongButtonY);
	AEVec2 animationTextCoordinate = ConvertPixelToNDC(animationButtonX, animationButtonY);

	AEGfxPrint(gameManager->m_font, pongButtonText.c_str(), pongTextCoordinate.x - pongButtonTextWidth / 2, pongTextCoordinate.y - pongButtonTextHeight / 2, TextScale, 1, 1, 1, 1);
	AEGfxPrint(gameManager->m_font, animationButtonText.c_str(), animationTextCoordinate.x - animationButtonTextWidth / 2, animationTextCoordinate.y - animationButtonTextHeight / 2, TextScale, 1, 1, 1, 1);
}

void MainMenuState::Exit(GameManager* gameManager)
{
}