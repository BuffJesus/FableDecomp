#include "fable_render_state_manager.h"
void CRenderManagerCore::DrawCurrentBuffersAsTris(const CTexture** textures,unsigned long count,unsigned long startVertex,unsigned long primitiveCount)
{
    for(unsigned long stage=0;stage<count;++stage)
    {
        IDirect3DBaseTexture9* texture=textures[stage]?textures[stage]->PD3DTexture:0;
        if(CurrentlySelectedTextures[stage]!=texture)
        {
            CurrentlySelectedTextures[stage]=texture;
            PD3DDevice->SetTexture(stage,texture);
        }
        if(TextureStagesInUse<(long)(stage+1))TextureStagesInUse=stage+1;
    }
    RealiseRenderState();
    PD3DDevice->DrawPrimitive(PT_TRIANGLE_LIST,startVertex,primitiveCount);
}
