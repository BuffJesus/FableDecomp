// lib_render_manager_2d.cpp, retail 0x009DDC60 (Ego_r
// ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z): a
// flat-coloured box in actual pixels at depth z, as triangles (0,1,2) and
// (0,2,3), with Draw2DTriangle's blend-mode rule.
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
#include "fable_2d_vector.h"

// See CRenderManager2D_Draw2DTriangle_009ddff0.cpp.
static inline unsigned long FABLE_FASTCALL MAKE_RGBA_LONG(unsigned long r, unsigned long g, unsigned long b, unsigned long a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

bool CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags)
{
    long blend_mode = 1;
    const unsigned long packed = MAKE_RGBA_LONG(colour.R, colour.G, colour.B, colour.A);
    C2DVector top_left(box.left, box.top);
    top_left.X = GFVXToAX(top_left.X);
    top_left.Y = GFVYToAY(box.top);
    const float left = top_left.X, top = top_left.Y;
    const float right = GFVXToAX(box.right);
    const float bottom = GFVYToAY(box.bottom);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = left; v[0].Y = top; v[0].SetZ(z);
    v[0].DiffuseColour = packed; v[0].U1 = 0.0f; v[0].V1 = 0.0f; v[0].SetRHWFromZ();
    v[1].X = right; v[1].Y = top; v[1].SetZ(z);
    v[1].DiffuseColour = packed; v[1].U1 = 0.0f; v[1].V1 = 0.0f; v[1].SetRHWFromZ();
    v[2].X = right; v[2].Y = bottom; v[2].SetZ(z);
    v[2].DiffuseColour = packed; v[2].U1 = 0.0f; v[2].V1 = 0.0f; v[2].SetRHWFromZ();
    v[3].X = left; v[3].Y = bottom; v[3].SetZ(z);
    v[3].DiffuseColour = packed; v[3].U1 = 0.0f; v[3].V1 = 0.0f; v[3].SetRHWFromZ();
    if (flags & 1)
    {
        if (colour.A != 255)
            blend_mode = 3;
    }
    else
        blend_mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], blend_mode, 0);
    Add2DTri(v[0], v[2], v[3], blend_mode, 0);
    return true;
}
