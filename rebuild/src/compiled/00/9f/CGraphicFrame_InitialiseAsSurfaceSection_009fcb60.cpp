#include "fable_graphic_frame.h"

bool CGraphicFrame::InitialiseAsSurfaceSection(
    const CCountedPointer<CTexture>& texture,
    float x, float y, long width, long height, bool alpha, bool booleanAlpha)
{
    FrameData = new CGraphicFrameData;
    return FrameData->InitialiseAsSurfaceSection(
        texture, x, y, width, height, alpha, booleanAlpha);
}
