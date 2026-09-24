#include "fable_ui_texture_surfaces.h"
void __fastcall FableUiReleaseTexture(CTexture* texture,void*)
{
    FableUiNativeTexture* native=FableUiNative(texture);
    if(native)
    {
        native->Vtable->Release(native);
        unsigned state=FableUiTextureState(texture)&0x0FFFFFFF;
        texture->PD3DTexture=0;
        FableUiSetTextureState(texture,state);
    }
}
