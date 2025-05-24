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

    AEMtx33 playerScale = { 0 };
    AEMtx33Scale(&playerScale, PADDLE_WIDTH, PADDLE_HEIGHT);

    AEMtx33 playerRotate = { 0 };
    AEMtx33Rot(&playerRotate, 0);

    AEMtx33 player1Translate = { 0 };
    AEMtx33Trans(&player1Translate, -700.0f, 0.0f);

    m_player1PaddleTransform = { 0 };
    AEMtx33Concat(&m_player1PaddleTransform, &playerRotate, &playerScale);
    AEMtx33Concat(&m_player1PaddleTransform, &player1Translate, &m_player1PaddleTransform);

    AEMtx33 player2Translate = { 0 };
    AEMtx33Trans(&player2Translate, 700.0f, 0.0f);

    m_player2PaddleTransform = { 0 };
    AEMtx33Concat(&m_player2PaddleTransform, &playerRotate, &playerScale);
    AEMtx33Concat(&m_player2PaddleTransform, &player2Translate, &m_player2PaddleTransform);


    AEMtx33 ballScale = { 0 };
    AEMtx33Scale(&ballScale, 50, 50);

    AEMtx33 ballRotate = { 0 };
    AEMtx33Rot(&ballRotate, 0);

    AEMtx33 ballTranslate = { 0 };
    AEMtx33Trans(&ballTranslate, 0.0f, 0.0f);

    m_ballTransform = { 0 };
    AEMtx33Concat(&m_ballTransform, &ballRotate, &ballScale);
    AEMtx33Concat(&m_ballTransform, &ballTranslate, &m_ballTransform);
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
    f32 current_text_scale;

    if (m_showTime)
    {
        sprintf_s(textBuffer, sizeof(textBuffer), "Time: %.1f s", m_elapsedTime);
        current_text_scale = 0.8f;

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        //draw player1
        AEGfxSetColorToMultiply(1.0f, 1.0f, 0.0f, 1.0f);
        AEGfxSetTransform(m_player1PaddleTransform.m);
        AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);

        //draw player2
        AEGfxSetColorToMultiply(0.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetTransform(m_player2PaddleTransform.m);
        AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);

        //draw ball
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetTransform(m_ballTransform.m);
        AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);
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