// lib_render_manager_2d.cpp, retail 0x009DDA50 (Ego_r
// ?Draw2DBoxColours@CRenderManager2D@@QAE_NABVC2DBoxF@@QBVCRGBColour@@K@Z): a
// box with one colour per corner (top-left, top-right, bottom-right,
// bottom-left), in actual pixels at depth zero. Flag 1 always blends (3);
// otherwise flag 2 is additive (2) and anything else solid (1).
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
#include "fable_2d_vector.h"

// See CRenderManager2D_Draw2DTriangle_009ddff0.cpp.
static inline unsigned long FABLE_FASTCALL MAKE_RGBA_LONG(unsigned long r, unsigned long g, unsigned long b, unsigned long a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

bool CRenderManager2D::Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* const colours, unsigned long flags)
{
    long blend_mode = 1;
    const unsigned long packed[4] = {
        MAKE_RGBA_LONG(colours[0].R, colours[0].G, colours[0].B, colours[0].A),
        MAKE_RGBA_LONG(colours[1].R, colours[1].G, colours[1].B, colours[1].A),
        MAKE_RGBA_LONG(colours[2].R, colours[2].G, colours[2].B, colours[2].A),
        MAKE_RGBA_LONG(colours[3].R, colours[3].G, colours[3].B, colours[3].A)};
    float left = box.left;
    left = GFVXToAX(left);
    const float top = GFVYToAY(box.top);
    const float right = GFVXToAX(box.right);
    const float bottom = GFVYToAY(box.bottom);
    CTVertexRHWColSpecTex1 v[4];
    v[0].U1 = 0.0f; v[0].V1 = 0.0f; v[0].DiffuseColour = packed[0]; v[0].SetZ(0.0f); v[0].SetRHWFromZ();
    v[1].U1 = 0.0f; v[1].V1 = 0.0f; v[1].DiffuseColour = packed[1]; v[1].SetZ(0.0f); v[1].SetRHWFromZ();
    v[2].U1 = 0.0f; v[2].V1 = 0.0f; v[2].DiffuseColour = packed[2]; v[2].SetZ(0.0f); v[2].SetRHWFromZ();
    v[3].U1 = 0.0f; v[3].V1 = 0.0f; v[3].DiffuseColour = packed[3]; v[3].SetZ(0.0f); v[3].SetRHWFromZ();
    v[0].X = left; v[0].Y = top;
    v[1].X = right; v[1].Y = top;
    v[2].X = right; v[2].Y = bottom;
    v[3].X = left; v[3].Y = bottom;
    if (flags & 1)
        blend_mode = 3;
    else
        blend_mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], blend_mode, 0);
    Add2DTri(v[0], v[2], v[3], blend_mode, 0);
    return true;
}
