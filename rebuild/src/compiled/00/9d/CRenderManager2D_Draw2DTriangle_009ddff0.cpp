// lib_render_manager_2d.cpp, retail 0x009DDFF0 (Ego_r
// ?Draw2DTriangle@CRenderManager2D@@QAE_NABVC2DVector@@00ABVCRGBColour@@K@Z):
// a flat-coloured triangle in actual pixels at depth zero. Blend mode: flag 1
// is alpha (3) unless the colour is opaque; otherwise flag 2 selects additive
// (2) and anything else solid (1).
#include "fable_display_viewport.h"
#include "fable_2d_vector.h"
#include "fable_virtual_pixels.h"

// The colour is packed byte by byte (a D3DCOLOR_ARGB-style macro in the
// original), not read as the whole CRGBColour dword.
static inline unsigned long FABLE_FASTCALL MAKE_RGBA_LONG(unsigned long r, unsigned long g, unsigned long b, unsigned long a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

bool CRenderManager2D::Draw2DTriangle(const C2DVector& p, const C2DVector& q, const C2DVector& r,
    const CRGBColour& colour, unsigned long flags)
{
    long blend_mode = 1;
    const unsigned long packed = MAKE_RGBA_LONG(colour.R, colour.G, colour.B, colour.A);
    CTVertexRHWColSpecTex1 v[3];
    v[0].X = GFVXToAX(p.X); v[0].Y = GFVYToAY(p.Y); v[0].SetZ(0.0f);
    v[0].DiffuseColour = packed; v[0].U1 = 0.0f; v[0].V1 = 0.0f; v[0].SetRHWFromZ();
    v[1].X = GFVXToAX(q.X); v[1].Y = GFVYToAY(q.Y); v[1].SetZ(0.0f);
    v[1].DiffuseColour = packed; v[1].U1 = 0.0f; v[1].V1 = 0.0f; v[1].SetRHWFromZ();
    v[2].X = GFVXToAX(r.X); v[2].Y = GFVYToAY(r.Y); v[2].SetZ(0.0f);
    v[2].DiffuseColour = packed; v[2].U1 = 0.0f; v[2].V1 = 0.0f; v[2].SetRHWFromZ();
    if (flags & 1)
    {
        if (colour.A != 255)
            blend_mode = 3;
    }
    else
        blend_mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], blend_mode, 0);
    return true;
}
