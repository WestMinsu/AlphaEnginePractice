#include "GameManager.h"
#include "Constants.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int gGameRunning = 1;

    AESysInit(hInstance, nCmdShow, kWindowWidth, kWindowHeight, 1, 60, true, NULL);
    AESysSetWindowTitle("Pong");
    AEFrameRateControllerInit(60);
    GameManager* gameManager = new GameManager();

    while (gGameRunning)
    {
        AESysFrameStart();

        gameManager->Update((f32)AEFrameRateControllerGetFrameTime());
        gameManager->Draw();

        AESysFrameEnd();

        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    delete gameManager;
    gameManager = nullptr;

    AESysExit();

    return 0;
}