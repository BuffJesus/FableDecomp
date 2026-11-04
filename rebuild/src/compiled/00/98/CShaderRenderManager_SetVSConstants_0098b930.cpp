#include "fable_shader_render_manager.h"
#include "fable_pixel_shader_data.h"
#include "fable_shader_constant_layout.h"
#include "fable_d3d9.h"
#include "fable_4d_vector.h"

typedef HRESULT (__stdcall *SetPixelShaderCall)(IDirect3DDevice9*,IDirect3DPixelShader9*);
typedef HRESULT (__stdcall *SetConstantsCall)(IDirect3DDevice9*,long,const void*,unsigned long);

void CShaderRenderManager::SetVSConstants(long startRegister,const void* data,unsigned long vectorCount)
{
    if(vectorCount)
    {
        void** methods=*reinterpret_cast<void***>(D3DDevice);
        reinterpret_cast<SetConstantsCall>(methods[94])(D3DDevice,startRegister,data,vectorCount);
    }
}
