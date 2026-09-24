#include "fable_ui_texture_surfaces.h"
bool __fastcall FableUiCreateBlankTexture(CTexture* texture,void*,const FableUiDisplayExtent* dimensions,int levels,const int* format,unsigned usage,unsigned pool,bool,unsigned)
{
    FableUiReleaseTexture(texture,0);
    if(levels<=0)
    {
        unsigned width=dimensions->Width,height=dimensions->Height;
        levels=1;
        while(width>=8 && height>=8) { width>>=1; height>>=1; ++levels; }
    }
    FableUiDisplayFormatView* display=static_cast<FableUiDisplayFormatView*>(FableUiGetSystemManager()->Display);
    FableUiNativeDevice* device=static_cast<FableUiNativeDevice*>(display->D3DDevice);
    if(device->Vtable->CreateTexture(device,dimensions->Width,dimensions->Height,levels,usage,
        FableUiPixelFormats[*format].D3DFormat,pool,&texture->PD3DTexture,0)<0) return false;
    FableUiSetTextureState(texture,(FableUiTextureState(texture)&0x0FFFFFFF)|0x10000000);
    FableUiUpdateTextureByteLength(texture,0);
    return true;
}
