#include "fable_shader_render_manager.h"
#include "fable_pixel_shader_data.h"
#include "fable_d3d9.h"

static inline void UninitialisePixelShader(CPixelShaderData*& slot)
{
    CPixelShaderData* data = slot;
    if (data)
        data->Release();
    slot = 0;
}

void CShaderRenderManager::DisablePixelShaders()
{
    if (PixelShadersEnabled)
    {
        // D3D9 SetPixelShader, slot 107; the minimal shared device interface
        // currently stops at Clear (slot 43).
        typedef HRESULT (__stdcall *SetPixelShaderCall)(IDirect3DDevice9*, IDirect3DPixelShader9*);
        void** methods = *reinterpret_cast<void***>(D3DDevice);
        reinterpret_cast<SetPixelShaderCall>(methods[107])(D3DDevice, 0);
        PixelShadersEnabled = false;
        updateFlags3D8 &= ~3u;
        UninitialisePixelShader(CurrentPixelShader.Data);
    }
}
