// lib_surface.cpp, retail 0x009F3470 / 0x009F3520 (Ego_r
// CSurface::CopyFromMemoryAutoSwizzle / CopyToMemoryAutoUnswizzle; the PC build
// has no swizzling): lock the surface and copy row by row between it and the
// caller's memory, each side stepping by its own pitch. Near matches, not
// gated: retail expands CSurface::GetPixelFormat itself (its temporaries set the
// frame) and counts rows down in the dead source_pitch slot, after hoisting
// row bytes = width * byte depth. The loop forms tried leave 20+ lines.
#include <string.h>
#include "fable_surface_types.h"

// Retail 009F3470.
void CSurface::CopyFromMemoryAutoSwizzle(void* source, long source_pitch)
{
    CSurfaceLockedRect rect = Lock(0);
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat format(desc.Format);
    unsigned long bytes_per_pixel = format.GetByteColourDepth();
    unsigned char* src = (unsigned char*)source;
    unsigned char* dst = rect.PMem;
    for (long y = rect.Dimensions.Y; y > 0; --y)
    {
        memcpy(dst, src, rect.Dimensions.X * bytes_per_pixel);
        src += source_pitch;
        dst += rect.Pitch;
    }
    PD3DSurface->UnlockRect();
}

// Retail 009F3520.
void CSurface::CopyToMemoryAutoUnswizzle(void* dest, long dest_pitch)
{
    CSurfaceLockedRect rect = Lock(0);
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat format(desc.Format);
    unsigned long bytes_per_pixel = format.GetByteColourDepth();
    unsigned char* src = rect.PMem;
    unsigned char* dst = (unsigned char*)dest;
    for (long y = rect.Dimensions.Y; y > 0; --y)
    {
        memcpy(dst, src, rect.Dimensions.X * bytes_per_pixel);
        src += rect.Pitch;
        dst += dest_pitch;
    }
    PD3DSurface->UnlockRect();
}
