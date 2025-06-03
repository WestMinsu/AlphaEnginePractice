#include "ResultState.h"
#include "GameManager.h"
#include <string>

void ResultState::Enter(GameManager* gameManager)
{

}

void ResultState::Update(GameManager* gameManager, f32 dt)
{
    if (AEInputCheckTriggered(AEVK_R))
    {
        gameManager->Initialize();
    }
}

void ResultState::Draw(GameManager* gameManager)
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    f32 win_w, win_h;
    AEGfxGetPrintSize(gameManager->m_font, gameManager->m_winMessage.c_str(), 1.5f, &win_w, &win_h);
    f32 win_x = -win_w / 2.0f;
    f32 win_y = 0.1f;

    if (gameManager->m_winMessage.find("PLAYER A") != std::string::npos) {
        AEGfxPrint(gameManager->m_font, gameManager->m_winMessage.c_str(), win_x, win_y, 1.5f, 1.0f, 1.0f, 0.0f, 1.0f);
    }
    else {
        AEGfxPrint(gameManager->m_font, gameManager->m_winMessage.c_str(), win_x, win_y, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f);
    }

    std::string restartMsg = "Press R to Restart";
    f32 restart_w, restart_h;
    AEGfxGetPrintSize(gameManager->m_font, restartMsg.c_str(), 1.0f, &restart_w, &restart_h);
    f32 restart_x = -restart_w / 2.0f;
    f32 restart_y = -0.1f;

    AEGfxPrint(gameManager->m_font, restartMsg.c_str(), restart_x, restart_y, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void ResultState::Exit(GameManager* gameManager)
{

}