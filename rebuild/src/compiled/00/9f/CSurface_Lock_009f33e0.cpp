#include "fable_ui_texture_surfaces.h"
FableUiSurfaceLock* __fastcall FableUiLockSurface(CSurface* surface,void*,FableUiSurfaceLock* out,unsigned flags)
{
    FableUiSurfaceDescription description;
    FableUiNativeSurface* native=FableUiNative(surface);
    native->Vtable->GetDescription(native,&description);
    native=FableUiNative(surface);
    FableUiRectangle rectangle={0,0,static_cast<int>(description.Width),static_cast<int>(description.Height)};
    FableUiLockRectangle lock;
    if(native->Vtable->Lock(native,&lock,&rectangle,flags)>=0)
    {
        out->Width=description.Width; out->Height=description.Height;
        out->Pitch=lock.Pitch; out->Bits=lock.Bits;
    }
    else
    {
        out->Width=0; out->Height=0; out->Pitch=0; out->Bits=0;
    }
    return out;
}
