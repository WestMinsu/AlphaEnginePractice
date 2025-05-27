#include "PongGame.h"
#include "AEUtil.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
       

const float kPaddleWidth = 50.0f;
const float kPaddleHeight = 200.0f;
const float kBallRadius = 50.0f;
const int kInvalidFontHandle = -1;

PongGame::PongGame()
    : m_elapsedTime(0.0),
    m_showTime(false),
    m_font(kInvalidFontHandle),
    m_randomEngine(std::random_device{}()),
    m_velocityDist(-1.0f, 1.0f)
{
    m_font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
    
    AEGfxMeshStart();
    AEGfxVertexAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
    AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
    AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
    AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
    AEGfxVertexAdd(0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f);
    AEGfxVertexAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
    m_mesh = AEGfxMeshEnd();

    m_pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");


    Initialize();
}

PongGame::~PongGame()
{
    if (m_font != kInvalidFontHandle)
    {
        AEGfxDestroyFont(m_font);
        m_font = kInvalidFontHandle;
        AEGfxMeshFree(m_mesh);
        AEGfxTextureUnload(m_pTex);
    }
}

void PongGame::Initialize()
{
    m_player1Position = { -700.f, 0.f };
    m_player2Position = { 700.f, 0.f };
    m_ballPosition = { 0.f, 0.f };

    m_elapsedTime = 0.0;
    m_showTime = false;
    m_ballSpeed = 500.0f; 

    f32 randomX = m_velocityDist(m_randomEngine);
    f32 randomY = m_velocityDist(m_randomEngine);

    m_ballVelocity.x = m_velocityDist(m_randomEngine);
    m_ballVelocity.y = m_velocityDist(m_randomEngine);

    AEVec2Normalize(&m_ballVelocity, &m_ballVelocity);

    m_ballVelocity.x *= m_ballSpeed;
    m_ballVelocity.y *= m_ballSpeed;
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
        m_ballPosition.x += m_ballVelocity.x * dt;
        m_ballPosition.y += m_ballVelocity.y * dt;
        m_elapsedTime += dt;
    }
}

void PongGame::Draw()
{
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    f32 w, h;
    std::string displayText;
    f32 currentTextScale;

    if (m_showTime)
    {
        std::stringstream ss;
        ss << "Time: " << std::fixed << std::setprecision(1) << m_elapsedTime << "s";
        displayText = ss.str();
        currentTextScale = 0.8f;

        //draw player1
        DrawRect(m_player1Position.x, m_player1Position.y, kPaddleWidth, kPaddleHeight, 1.0f, 1.0f, 0.0f, 1.0f);
       
        //draw player2
        DrawRect(m_player2Position.x, m_player2Position.y, kPaddleWidth, kPaddleHeight, 0.0f, 1.0f, 1.0f, 1.0f);
    
        //draw ball
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);
        AEGfxTextureSet(m_pTex, 0, 0);
        //std::cout << m_ballPosition.x << " " << m_ballPosition.y << std::endl;
        DrawRect(m_ballPosition.x, m_ballPosition.y, kBallRadius, kBallRadius, 1.0f, 1.0f, 1.0f, 1.0f, m_pTex);
    }
    else
    {
        displayText = "Press SPACE key to start...";
        currentTextScale = 1.2f;
    }

    AEGfxGetPrintSize(m_font, displayText.c_str(), currentTextScale, &w, &h);

    f32 textXPosition = -w / 2;
    f32 textYPosition;
    if (m_showTime) 
    {
        textYPosition = 0.9f - h;
    }
    else
    {
        textYPosition = -h / 2;
    }

    AEGfxPrint(m_font, displayText.c_str(), textXPosition, textYPosition, currentTextScale, 1, 1, 1, 1);
}

void PongGame::DrawRect(f32 x, f32 y, f32 w, f32 h, float r, float g, float b, float a, AEGfxTexture* pTex)
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

    if(pTex == nullptr)
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    else
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    AEGfxSetColorToMultiply(r, g, b, a);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

    AEGfxTextureSet(pTex, 0, 0);

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(m_mesh, AE_GFX_MDM_TRIANGLES);
}
