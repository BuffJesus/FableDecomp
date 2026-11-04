// lib_render_manager_2d.cpp, retail 0x009DC4C0-0x009DC577 (Ego_r
// CRenderManager2D::AddLineToDrawBuffer / AddTriToDrawBuffer /
// AddQuadToDrawBuffer): blend mode 1 (solid) goes straight to the opaque
// buffers (AddTri 009DB810); every blended mode goes to the sorted alpha list
// (AddAlphaTri 009DBD10). A quad is two triangles (a,b,c),(a,c,d).
#include "fable_display_viewport.h"

// Retail 009DC4C0.
void CRenderManager2D::AddLineToDrawBuffer(const CTVertexRHWColSpecTex1& a, const CTVertexRHWColSpecTex1& b,
                                           unsigned long blend_mode, CTexture* texture)
{
    if (blend_mode == 1)
        AddLine(a, b, 1, texture);
    else
        AddLine(a, b, blend_mode, texture);
}

// Retail 009DC4E0.
void CRenderManager2D::AddTriToDrawBuffer(const CTVertexRHWColSpecTex1& a, const CTVertexRHWColSpecTex1& b,
                                          const CTVertexRHWColSpecTex1& c, unsigned long blend_mode, CTexture* texture)
{
    if (blend_mode == 1)
        AddTri(a, b, c, blend_mode, texture);
    else
        AddAlphaTri(a, b, c, blend_mode, texture);
}

// Retail 009DC500.
void CRenderManager2D::AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1& a, const CTVertexRHWColSpecTex1& b,
                                           const CTVertexRHWColSpecTex1& c, const CTVertexRHWColSpecTex1& d,
                                           unsigned long blend_mode, CTexture* texture)
{
    if (blend_mode == 1)
    {
        AddTri(a, b, c, 1, texture);
        AddTri(a, c, d, 1, texture);
    }
    else
    {
        AddAlphaTri(a, b, c, blend_mode, texture);
        AddAlphaTri(a, c, d, blend_mode, texture);
    }
}
