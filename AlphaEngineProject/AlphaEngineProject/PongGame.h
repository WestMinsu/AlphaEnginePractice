#pragma once
#include <crtdbg.h>
#include "AEEngine.h"

class PongGame
{
public:

    PongGame();
   
    ~PongGame();

    void Initialize();

    void Update(f32 dt);

    void Draw();

private:
    f64 m_elapsedTime;       
    bool m_showTime;         
    s8 m_font;     
    AEGfxVertexList* m_mesh;

    AEMtx33 m_player1PaddleTransform;
    AEMtx33 m_player2PaddleTransform;
    AEMtx33 m_ballTransform;
};
