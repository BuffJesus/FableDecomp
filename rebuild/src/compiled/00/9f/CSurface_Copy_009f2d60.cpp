#include "fable_ui_texture_surfaces.h"
CSurface* __fastcall FableUiCopySurface(CSurface* out,void*,const CSurface* source)
{
    out->__vftable=FableUiSurfaceVtable;
    out->PD3DSurface=source->PD3DSurface;
    out->AllocationSource=source->AllocationSource;
    if(out->AllocationSource==2) out->PAllocatedMemory=source->PAllocatedMemory;
    FableUiNativeSurface* native=FableUiNative(out);
    if(native) native->Vtable->AddRef(native);
    return out;
}
