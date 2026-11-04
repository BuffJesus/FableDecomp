#include "fable_render_state_manager.h"
void CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(const CTexture** textures,unsigned long count,unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount)
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
    PD3DDevice->DrawIndexedPrimitive(PT_TRIANGLE_LIST,0,minVertex,vertexCount,startIndex,primitiveCount);
}
