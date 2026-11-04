// lib_render_manager_2d.cpp, retail 0x009DE3F0 (Ego_r
// ?Draw2DBox@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z): a filled
// box at depth zero.
#include "fable_display_viewport.h"

bool CRenderManager2D::Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags)
{
    return Draw2DBoxWithZ(box, 0.0f, colour, flags);
}
