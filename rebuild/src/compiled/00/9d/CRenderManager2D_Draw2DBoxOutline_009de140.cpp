// lib_render_manager_2d.cpp, retail 0x009DE140 (Ego_r
// ?Draw2DBoxOutline@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z):
// the box's edges, snapped to whole virtual units, with the right and bottom
// edges pulled in by one actual pixel; true only if all four lines drew.
#include "fable_display_viewport.h"
#include "fable_2d_vector.h"
#include "fable_virtual_pixels.h"

bool CRenderManager2D::Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags)
{
    const long top = (long)box.top;
    const long left = (long)box.left;
    const long right = (long)(box.right - GFAXToVX(1.0f));
    const long bottom = (long)(box.bottom - GFAYToVY(1.0f));
    if (Draw2DLine(C2DVector((float)left, (float)top), C2DVector((float)right, (float)top), colour, flags) == true &&
        Draw2DLine(C2DVector((float)right, (float)top), C2DVector((float)right, (float)bottom), colour, flags) == true &&
        Draw2DLine(C2DVector((float)right, (float)bottom), C2DVector((float)left, (float)bottom), colour, flags) == true &&
        Draw2DLine(C2DVector((float)left, (float)bottom), C2DVector((float)left, (float)top), colour, flags) == true)
        return true;
    return false;
}
