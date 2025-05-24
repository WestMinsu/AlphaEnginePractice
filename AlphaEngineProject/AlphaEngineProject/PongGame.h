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

    void DrawRect(f32 x, f32 y, f32 w, f32 h, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

private:
    f64 m_elapsedTime;       
    bool m_showTime;         
    s8 m_font;     
    AEGfxVertexList* m_mesh;
};
