// lib_render_manager_2d.cpp, retail 0x009DD960 (Ego_r
// ?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z): a
// flat-coloured line in actual pixels at depth zero (specular cleared), with
// Draw2DTriangle's blend-mode rule. The statement order of the field
// assignments is the one VC7.1 schedules into retail's store sequence.
#include "fable_display_viewport.h"
#include "fable_2d_vector.h"
#include "fable_virtual_pixels.h"

// See CRenderManager2D_Draw2DTriangle_009ddff0.cpp.
static inline unsigned long FABLE_FASTCALL MAKE_RGBA_LONG(unsigned long r, unsigned long g, unsigned long b, unsigned long a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

bool CRenderManager2D::Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags)
{
    long blend_mode = 1;
    const unsigned long packed = MAKE_RGBA_LONG(colour.R, colour.G, colour.B, colour.A);
    CTVertexRHWColSpecTex1 a, b;
    a.X = GFVXToAX(from.X); a.Y = GFVYToAY(from.Y);
    b.X = GFVXToAX(to.X); b.Y = GFVYToAY(to.Y);
    a.DiffuseColour = packed; b.DiffuseColour = packed;
    a.SpecularColour = 0; b.SpecularColour = 0;
    a.SetZ(0.0f); b.SetZ(0.0f);
    a.SetRHWFromZ(); b.SetRHWFromZ();
    b.U1 = 0.0f; a.U1 = 0.0f;
    b.V1 = 0.0f; a.V1 = 0.0f;
    if (flags & 1)
    {
        if (colour.A != 255)
            blend_mode = 3;
    }
    else
        blend_mode = (flags & 2) ? 2 : 1;
    Add2DLine(a, b, blend_mode, 0);
    return true;
}
