#include "fable_shader_render_manager.h"
#include "fable_pixel_shader_data.h"
#include "fable_shader_constant_layout.h"
#include "fable_d3d9.h"
#include "fable_4d_vector.h"

typedef HRESULT (__stdcall *SetPixelShaderCall)(IDirect3DDevice9*,IDirect3DPixelShader9*);
typedef HRESULT (__stdcall *SetConstantsCall)(IDirect3DDevice9*,long,const void*,unsigned long);

void CShaderRenderManager::UpdateAmbient()
{
    C4DVector ambient;
    ambient.X=AmbientColour.R;
    ambient.Y=AmbientColour.G;
    ambient.Z=AmbientColour.B;
    ambient.W=1.0f;
    void** methods=*reinterpret_cast<void***>(D3DDevice);
    reinterpret_cast<SetConstantsCall>(methods[94])(D3DDevice,CurrentVSConstantLayout->Ambient.Offset,&ambient,1);
    updateFlags3D8 &= ~4u;
}
