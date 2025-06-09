#include "Utility.h"
AEVec2 ConvertPixelToNDC(f32 pixelX, f32 pixelY)
{
    AEVec2 ndcCoord;
    ndcCoord.x = (2.0f * pixelX / kWindowWidth);
    ndcCoord.y = (2.0f * pixelY / kWindowHeight);
    return ndcCoord;
}