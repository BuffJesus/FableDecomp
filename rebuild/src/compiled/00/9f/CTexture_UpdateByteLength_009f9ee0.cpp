#include "fable_ui_texture_surfaces.h"
void __fastcall FableUiUpdateTextureByteLength(CTexture* texture,void*)
{
    FableUiSetTextureState(texture,FableUiTextureState(texture)&0xF0000000);
    FableUiNativeTexture* native=FableUiNative(texture);
    if(!native) return;
    int count=static_cast<int>(native->Vtable->GetLevelCount(native));
    for(int level=0;level<count;++level)
    {
        FableUiSurfaceDescription dimensions,formatDescription;
        native=FableUiNative(texture);
        native->Vtable->GetLevelDescription(native,level,&dimensions);
        native=FableUiNative(texture);
        native->Vtable->GetLevelDescription(native,0,&formatDescription);
        int format;
        FableUiSetPixelFormat(&format,0,formatDescription.Format);
        unsigned state=FableUiTextureState(texture);
        unsigned size=static_cast<unsigned>(FableUiPixelFormatBits(&format,0));
        size*=dimensions.Height; size*=dimensions.Width;
        FableUiSetTextureState(texture,(state&0xF0000000)|((state+(size>>3))&0x0FFFFFFF));
    }
}
