#pragma once
#include <crtdbg.h>
#include "AEEngine.h"

extern f64 g_elapsedTime;
extern bool g_showTime;
extern s8 font; 

void PongGameInit(void);

void PongGameUpdate(f32 dt); 

void PongGameDraw(void);

void PongGameExit(void);

void PongGameReset(void);
