#include "PongGame.h" 
#include <cstdio>    

f64 g_elapsedTime = 0.0;
bool g_showTime = false;
s8 font = -1; 

void PongGameInit(void)
{
    font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72.f);

    PongGameReset();
}

void PongGameUpdate(f32 dt) 
{
    if (AEInputCheckTriggered(AEVK_SPACE))
    {
        if (!g_showTime) {
            g_showTime = true;
        }
    }

    if (AEInputCheckTriggered(AEVK_R))
    {
        PongGameReset();
    }

    if (g_showTime)
    {
        g_elapsedTime += dt;
    }
}

void PongGameDraw(void)
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    char textBuffer[256];
    const char* initialMessage = "Press SPACE key to start...";

    f32 w, h; 
    f32 current_text_scale;

    if (g_showTime)
    {
        sprintf_s(textBuffer, sizeof(textBuffer), "Time: %.1f s", g_elapsedTime);
        current_text_scale = 0.8f;
    }
    else
    {
        strcpy_s(textBuffer, sizeof(textBuffer), initialMessage);
        current_text_scale = 1.2f; 
    }

    AEGfxGetPrintSize(font, textBuffer, current_text_scale, &w, &h);

    f32 text_x_pos = -w / 2;

    f32 text_y_pos;
    if (g_showTime) 
    {
        text_y_pos = 0.9f - h;
    }
    else 
    {
        text_y_pos = -h / 2;
    }

    AEGfxPrint(font, textBuffer, text_x_pos, text_y_pos, current_text_scale, 1, 1, 1, 1);
}


void PongGameExit(void)
{
    if (font != -1) {
        AEGfxDestroyFont(font);
        font = -1;
    }
}

void PongGameReset(void)
{
    g_elapsedTime = 0.0;
    g_showTime = false;

}