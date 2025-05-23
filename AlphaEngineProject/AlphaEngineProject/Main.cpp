#include "PongGame.h" 

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int gGameRunning = 1;

    AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);
    AESysSetWindowTitle("Pong");
    AEFrameRateControllerInit(60);

    PongGame* pongGame = new PongGame(); 

    // Game Loop
    while (gGameRunning)
    {
        AESysFrameStart();

        pongGame->Update((f32)AEFrameRateControllerGetFrameTime());
        pongGame->Draw();

        AESysFrameEnd();

        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    delete pongGame;
    pongGame = nullptr;

    AESysExit();

    return 0;
}