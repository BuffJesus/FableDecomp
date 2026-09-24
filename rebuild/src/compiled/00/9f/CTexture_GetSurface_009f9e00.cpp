#include "fable_ui_texture_surfaces.h"
CSurface* __fastcall FableUiGetTextureSurface(CTexture* texture,void*,CSurface* out,unsigned level)
{
    FableUiNativeTexture* native=FableUiNative(texture);
    IDirect3DSurface9* returned;
    native->Vtable->GetSurfaceLevel(native,level,&returned);
    CSurface local;
    local.__vftable=FableUiSurfaceVtable;
    local.PD3DSurface=0;
    local.AllocationSource=0;
    FableUiAttachSurface(&local,0,returned);
    FableUiCopySurface(out,0,&local);
    local.__vftable=FableUiSurfaceVtable;
    FableUiReleaseSurface(&local,0);
    return out;
}
