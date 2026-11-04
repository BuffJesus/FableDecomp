// lib_render_manager_2d.cpp, retail 0x009DC2C0 (Ego_r ?Add2DLine@CRenderManager2D@@...):
// queue a two-vertex line in clip space with its blend mode, texture and
// current window. Validated work-only on 2026-10-13 (36,864-case native
// behaviour fixture); landed unchanged against the shared header.
#include <new>
#include "fable_display_viewport.h"
namespace _STL {
template<> void vector<CTVertexRHWColSpecTex1>::_M_insert_overflow(CTVertexRHWColSpecTex1*,
    const CTVertexRHWColSpecTex1&, const __false_type&, size_t, bool);
template<> void vector<CQuickDrawTriInfo>::_M_insert_overflow(CQuickDrawTriInfo*,
    const CQuickDrawTriInfo&, const __false_type&, size_t, bool);
}
#include "fable_stlport_vector_impl.h"
void CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1& a,
    const CTVertexRHWColSpecTex1& b,
    long blendMode, CTexture* texture)
{
    if (DisplayManager->ViewportOffscreen)
        return;
    CTVertexRHWColSpecTex1 vertex;
    vertex = a;
    {
        const C2DBoxF& window = GetCurrentAWindow();
        vertex.X = 2.0f * (vertex.X / (window.right - window.left)) - 1.0f;
    }
    {
        const C2DBoxF& window = GetCurrentAWindow();
        vertex.Y = 1.0f - 2.0f * (vertex.Y / (window.bottom - window.top));
    }
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    QuickDraw2DVertices.push_back(vertex);
    vertex = b;
    {
        const C2DBoxF& window = GetCurrentAWindow();
        vertex.X = 2.0f * (vertex.X / (window.right - window.left)) - 1.0f;
    }
    {
        const C2DBoxF& window = GetCurrentAWindow();
        vertex.Y = 1.0f - 2.0f * (vertex.Y / (window.bottom - window.top));
    }
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    QuickDraw2DVertices.push_back(vertex);
    CQuickDrawTriInfo info;
    info.BlendMode = blendMode;
    if (texture)
        info.UploadedTexture = *texture;
    info.Window = GetCurrentAWindow();
    info.Line = true;
    info.IsText = false;
    QuickDraw2DTriInfo.push_back(info);
}
