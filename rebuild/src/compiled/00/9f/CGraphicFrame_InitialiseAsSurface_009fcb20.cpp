#include "fable_graphic_frame.h"
#include "fable_graphic_frame_data.h"

bool CGraphicFrame::InitialiseAsSurface(const CCountedPointer<CTexture>& texture,
    long width, long height, bool alpha, bool booleanAlpha)
{
    FrameData = new CGraphicFrameData;
    return FrameData->InitialiseAsSurface(texture, width, height, alpha, booleanAlpha);
}
