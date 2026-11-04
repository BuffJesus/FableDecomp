// lib_render_manager_core.cpp, retail 0x00A0AA60, 0x00A0AAB0 and 0x00A0ACA0-0x00A0AE9F
// (Ego_r CRenderManagerCore::IsRendering, GetCurrentAWindowInt, DrawCurrentBuffersIndexed,
// DrawCurrentBuffersAsTriFan, DrawCurrentBuffersAsPointList).
#include "fable_display_viewport.h"
#include "engine/CIndexBuffer.h"

// Retail 00A0AA60 (the body is shared by identical-code folding with an
// inventory query, which the index still lists under a stand-in name).
bool CRenderManagerCore::IsRendering()
{
    return Rendering;
}

// Retail 00A0AAB0.
const C2DBoxI& CRenderManagerCore::GetCurrentAWindowInt() const
{
    return DisplayManager->ViewportBox;
}

// Retail 00A0AD40: the primitive type comes from the selected index buffer
// (read before the render state is realised).
void CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long minVertex, unsigned long vertexCount,
    unsigned long startIndex, unsigned long primitiveCount)
{
    EPrimitiveType type = PSelectedIndexBuffer->PrimitiveType;
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitive(type, 0, minVertex, vertexCount, startIndex, primitiveCount);
}

// Retail 00A0ACA0: bind any changed stage textures first.
void CRenderManagerCore::DrawCurrentBuffersIndexed(const CTexture** textures, unsigned long count,
    unsigned long minVertex, unsigned long vertexCount, unsigned long startIndex, unsigned long primitiveCount)
{
    for (unsigned long stage = 0; stage < count; ++stage)
    {
        IDirect3DBaseTexture9* texture = textures[stage] ? textures[stage]->PD3DTexture : 0;
        if (CurrentlySelectedTextures[stage] != texture)
        {
            CurrentlySelectedTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (TextureStagesInUse < (long)(stage + 1))
            TextureStagesInUse = stage + 1;
    }
    EPrimitiveType type = PSelectedIndexBuffer->PrimitiveType;
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitive(type, 0, minVertex, vertexCount, startIndex, primitiveCount);
}

// Retail 00A0AE40.
void CRenderManagerCore::DrawCurrentBuffersAsTriFan(long vertexCount, long primitiveCount)
{
    RealiseRenderState();
    PD3DDevice->DrawPrimitive(PT_TRIANGLE_FAN, 0, primitiveCount);
}

// Retail 00A0AE70.
void CRenderManagerCore::DrawCurrentBuffersAsPointList(long vertexCount, long primitiveCount)
{
    RealiseRenderState();
    PD3DDevice->DrawPrimitive(PT_POINT_LIST, 0, primitiveCount);
}
