#include "fable_shader_render_manager.h"
#include "fable_pixel_shader_data.h"
#include "fable_d3d9.h"
#include "fable_4d_vector.h"

void CShaderRenderManager::UpdatePixelShaderConstants()
{
    CPixelShaderData* shaderData=CurrentPixelShader.Data;
    if(shaderData==0)
        goto clearUpdateFlag;
    {
        const unsigned long count=shaderData->ConstCount;
        const long* indices=shaderData->ConstIndices;
        static C4DVector ps_constants[8];
        for(unsigned long index=0;index<count;++index)
            ps_constants[index]=PSConstTable._M_start[indices[index]];
        if(count)
        {
            typedef HRESULT (__stdcall *SetConstantsCall)(IDirect3DDevice9*,long,const void*,unsigned long);
            void** methods=*reinterpret_cast<void***>(D3DDevice);
            reinterpret_cast<SetConstantsCall>(methods[109])(D3DDevice,0,ps_constants,count);
        }
    }
clearUpdateFlag:
    updateFlags3D8 &= ~2u;
}
