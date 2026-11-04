#include "fable_2d_vector.h"
#include "fable_display_viewport.h"

bool CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale,
    CWideString text, const CFontBank& font, const CRGBColour& colour,
    unsigned long flags)
{
    AddText(position, scale, text, font, colour, flags);
    return true;
}

// Retail 009DD8F0 (Ego_r ?Draw2DText@...): flat text at depth zero, unit scale.
bool CRenderManager2D::Draw2DText(const C2DVector& position, CWideString text, const CFontBank& font,
    const CRGBColour& colour, unsigned long flags)
{
    return Draw3DScaledText(C3DVector(position.X, position.Y, 0.0f), 1.0f, text, font, colour, flags);
}
