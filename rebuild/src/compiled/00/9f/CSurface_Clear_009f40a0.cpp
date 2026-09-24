#include "fable_ui_texture_surfaces.h"
void __fastcall FableUiClearSurface(CSurface* surface,void*)
{
    FableUiSurfaceLock lock;
    FableUiLockSurface(surface,0,&lock,0);
    FableUiSurfaceDescription dimensions,formatDescription;
    FableUiNativeSurface* native=FableUiNative(surface);
    native->Vtable->GetDescription(native,&dimensions);
    native=FableUiNative(surface);
    native->Vtable->GetDescription(native,&formatDescription);
    int format;
    FableUiSetPixelFormat(&format,0,formatDescription.Format);
    unsigned size=static_cast<unsigned>(FableUiPixelFormatBits(&format,0));
    size*=dimensions.Height; size*=dimensions.Width;
    // Retail clears a contiguous byte count, not pitch * height.
    memset(lock.Bits,0,size>>3);
    native=FableUiNative(surface);
    native->Vtable->Unlock(native);
}
