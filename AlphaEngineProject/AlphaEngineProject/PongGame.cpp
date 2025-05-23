#include "PongGame.h"
#include <cstdio> 


PongGame::PongGame()
    : m_elapsedTime(0.0),
    m_showTime(false),
    m_font(-1)
{

    m_font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72.f);

    Initialize();
}


PongGame::~PongGame()
{
    if (m_font != -1) {
        AEGfxDestroyFont(m_font);
        m_font = -1;
    }

}

void PongGame::Initialize()
{
    m_elapsedTime = 0.0;
    m_showTime = false;
}

void PongGame::Update(f32 dt)
{
    if (AEInputCheckTriggered(AEVK_SPACE))
    {
        if (!m_showTime) {
            m_showTime = true;
        }
    }

    if (AEInputCheckTriggered(AEVK_R))
    {
        Initialize();
    }

    if (m_showTime)
    {
        m_elapsedTime += dt;
    }
}

void PongGame::Draw()
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    char textBuffer[256];
    const char* initialMessage = "Press SPACE key to start...";

    f32 w, h;
    f32 current_text_scale;

    if (m_showTime)
    {
        sprintf_s(textBuffer, sizeof(textBuffer), "Time: %.1f s", m_elapsedTime);
        current_text_scale = 0.8f;
    }
    else
    {
        strcpy_s(textBuffer, sizeof(textBuffer), initialMessage);
        current_text_scale = 1.2f;
    }

    AEGfxGetPrintSize(m_font, textBuffer, current_text_scale, &w, &h);

    f32 text_x_pos = -w / 2;
    f32 text_y_pos;
    if (m_showTime) {
        text_y_pos = 0.9f - h;
    }
    else {
        text_y_pos = -h / 2;
    }

    AEGfxPrint(m_font, textBuffer, text_x_pos, text_y_pos, current_text_scale, 1, 1, 1, 1);
}