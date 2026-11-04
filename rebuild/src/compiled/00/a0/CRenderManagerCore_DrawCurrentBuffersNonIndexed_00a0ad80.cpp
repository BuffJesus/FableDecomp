// lib_render_manager_core.cpp, retail 0x00A0AD80 (Ego_r
// ?DrawCurrentBuffersNonIndexed@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKW4EPrimitiveType@@@Z):
// bind any changed stage textures, flush render state, then one
// DrawPrimitive of the current vertex buffer.
#include "fable_render_state_manager.h"

void CRenderManagerCore::DrawCurrentBuffersNonIndexed(const CTexture** textures, unsigned long count,
    unsigned long startVertex, unsigned long primitiveCount, EPrimitiveType primitiveType)
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
    RealiseRenderState();
    PD3DDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);
}
