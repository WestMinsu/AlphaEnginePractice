#include "MainMenuState.h"
#include "GameManager.h"
#include <string>

void MainMenuState::Enter(GameManager* gameManager)
{
}

void MainMenuState::Update(GameManager* gameManager, f32 dt)
{
    if (AEInputCheckTriggered(AEVK_SPACE))
        gameManager->ChangeState(GameState::MAIN_GAME);
}

void MainMenuState::Draw(GameManager* gameManager)
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    std::string displayText = "Press SPACE key to start...";
    f32 TextScale = 1.2f;

    f32 w, h;
    AEGfxGetPrintSize(gameManager->m_font, displayText.c_str(), TextScale, &w, &h);

    f32 textXPosition = -w / 2;
    f32 textYPosition = -h / 2;

    AEGfxPrint(gameManager->m_font, displayText.c_str(), textXPosition, textYPosition, TextScale, 1, 1, 1, 1);
}

void MainMenuState::Exit(GameManager* gameManager)
{
}