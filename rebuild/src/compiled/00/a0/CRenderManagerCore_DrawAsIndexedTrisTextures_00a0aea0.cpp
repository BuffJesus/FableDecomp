#include "fable_render_state_manager.h"
void CRenderManagerCore::DrawAsIndexedTris(const CTexture** textures,unsigned long count,const void* vertices,long stride,long vertexCount,void* indices,long primitiveCount)
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
    PD3DDevice->DrawIndexedPrimitiveUP(PT_TRIANGLE_LIST,0,vertexCount,primitiveCount,indices,D3DFMT_INDEX16,vertices,stride);
}
