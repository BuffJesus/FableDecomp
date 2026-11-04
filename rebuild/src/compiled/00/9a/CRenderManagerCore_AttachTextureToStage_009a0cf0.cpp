#include "fable_render_state_manager.h"

void CRenderManagerCore::AttachTextureToStage(long stage,const CTexture* texture)
{
    IDirect3DBaseTexture9* deviceTexture =
        texture != 0
            ? texture->PD3DTexture
            : 0;
    if (CurrentlySelectedTextures[stage] != deviceTexture)
    {
        CurrentlySelectedTextures[stage] = deviceTexture;
        typedef HRESULT (__stdcall* SetTextureCall)(IDirect3DDevice9*,unsigned long,IDirect3DBaseTexture9*);
        void** vtable=*reinterpret_cast<void***>(PD3DDevice);
        reinterpret_cast<SetTextureCall>(vtable[65])(
                PD3DDevice,
                stage,
                deviceTexture);
    }
    if (TextureStagesInUse < stage + 1)
    {
        TextureStagesInUse = stage + 1;
    }
}
