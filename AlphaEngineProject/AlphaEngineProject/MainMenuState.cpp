#include "MainMenuState.h"
#include "GameManager.h"
#include <string>
#include "Constants.h"

void MainMenuState::Enter(GameManager* gameManager)
{
}

void MainMenuState::Update(GameManager* gameManager, f32 dt)
{
	if (AEInputCheckReleased(AEVK_LBUTTON)) // && click button
		gameManager->ChangeState(GameState::MAIN_GAME);
}

void MainMenuState::Draw(GameManager* gameManager)
{
	f32 buttonWidth = 350.f;
	f32 buttonHeight = 100.f;

	f32 pongButtonX = 0.f;
	f32 pongButtonY = 200.f;

	f32 animationButtonX = 0.f;
	f32 animationButtonY = -200.f;

	AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);
	gameManager->DrawRect(pongButtonX, pongButtonY, buttonWidth, buttonHeight, 0.5, 0.5, 0.5, 1);
	gameManager->DrawRect(animationButtonX, animationButtonY, buttonWidth, buttonHeight, 0.5, 0.5, 0.5, 1);

	std::string pongButtonText = "Pong";
	std::string animationButtonText = "Animation";
	f32 TextScale = 0.9f;

	f32 w, h;
	AEGfxGetPrintSize(gameManager->m_font, pongButtonText.c_str(), TextScale, &w, &h);
	f32 w2, h2;
	AEGfxGetPrintSize(gameManager->m_font, animationButtonText.c_str(), TextScale, &w2, &h2);

	f32 pongTextX = 2 * pongButtonX / kGameWindowWidth - w / 2;
	f32 pongtextY = 2 * pongButtonY / kGameWindowHeight - h / 2;

	f32 animationTextX = 2 * animationButtonX / kGameWindowWidth - w2 / 2;
	f32 animationTextY = 2 * animationButtonY / kGameWindowHeight - h2 / 2;

	AEGfxPrint(gameManager->m_font, pongButtonText.c_str(), pongTextX, pongtextY, TextScale, 1, 1, 1, 1);
	AEGfxPrint(gameManager->m_font, animationButtonText.c_str(), animationTextX, animationTextY, TextScale, 1, 1, 1, 1);
}

void MainMenuState::Exit(GameManager* gameManager)
{
}