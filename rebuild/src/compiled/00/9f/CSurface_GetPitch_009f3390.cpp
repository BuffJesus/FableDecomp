// lib_surface.cpp, retail 0x009F3390 (Ego_r ?GetPitch@CSurface@@QBEKXZ): bytes
// per row, from the format's colour depth and the width. GetPixelFormat and
// GetDimensions are expanded (each its own GetDesc), as in GetByteLength.
#include "fable_surface_types.h"

unsigned long CSurface::GetPitch() const
{
    D3DSURFACE_DESC descriptor;
    PD3DSurface->GetDesc(&descriptor);
    CPixelFormat format(descriptor.Format);
    D3DSURFACE_DESC dimensions;
    PD3DSurface->GetDesc(&dimensions);
    unsigned long width = dimensions.Width;
    return format.GetColourDepth() * width / 8;
}
