#include "PongGame.h"
#include <cstdio> 

#define PADDLE_WIDTH  50.0f
#define PADDLE_HEIGHT 200.0f
#define BALL_SIZE     40.0f


PongGame::PongGame()
    : m_elapsedTime(0.0),
    m_showTime(false),
    m_font(-1)
{
    m_font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72.f);
    
    AEGfxMeshStart();
    AEGfxVertexAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
    AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
    AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
    AEGfxVertexAdd(0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
    AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
    m_mesh = AEGfxMeshEnd();

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
        if (!m_showTime) 
        {
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
    f32 currentTextScale;

    if (m_showTime)
    {
        sprintf_s(textBuffer, sizeof(textBuffer), "Time: %.1f s", m_elapsedTime);
        currentTextScale = 0.8f;

        //draw player2
        DrawRect(-700.0f, 0.0f, 50.0f, 200.0f, 1.0f, 1.0f, 0.0f, 1.0f);
       
        //draw player2
        DrawRect(700.0f, 0.0f, 50.0f, 200.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    
        //draw ball
        DrawRect(0.0f, 0.0f, 50.0f, 50.0f);
    }
    else
    {
        strcpy_s(textBuffer, sizeof(textBuffer), initialMessage);
        currentTextScale = 1.2f;
    }

    AEGfxGetPrintSize(m_font, textBuffer, currentTextScale, &w, &h);

    f32 textXPosition = -w / 2;
    f32 textYPosition;
    if (m_showTime) {
        textYPosition = 0.9f - h;
    }
    else {
        textYPosition = -h / 2;
    }

    AEGfxPrint(m_font, textBuffer, textXPosition, textYPosition, currentTextScale, 1, 1, 1, 1);
}

void PongGame::DrawRect(f32 x, f32 y, f32 w, f32 h, float r, float g, float b, float a)
{
    AEMtx33 scale = { 0 };
    AEMtx33Scale(&scale, w, h);

    AEMtx33 rotate = { 0 };
    AEMtx33Rot(&rotate, 0);

    AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, x, y);

    AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    AEGfxSetColorToMultiply(r, g, b, a);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);
}
