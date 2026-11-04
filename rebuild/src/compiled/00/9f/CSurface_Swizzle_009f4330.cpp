// lib_surface.cpp, retail 0x009F2E40 / 0x009F2E50 / 0x009F4330 / 0x009F4360
// (Ego_r CSurface::Swizzle / Unswizzle): PC surfaces are linear, so the
// CSurfaceLockedRect forms do nothing. The flag forms lock, call them
// (expanded away) and unlock.
#include "fable_surface_types.h"

// Retail 009F2E40.
void CSurface::Swizzle(CSurfaceLockedRect& lock)
{
}

// Retail 009F2E50.
void CSurface::Unswizzle(CSurfaceLockedRect& lock)
{
}

// Retail 009F4330.
void CSurface::Swizzle(unsigned long flags)
{
    CSurfaceLockedRect lock = Lock(flags);
    Swizzle(lock);
    PD3DSurface->UnlockRect();
}

// Retail 009F4360.
void CSurface::Unswizzle(unsigned long flags)
{
    CSurfaceLockedRect lock = Lock(flags);
    Unswizzle(lock);
    PD3DSurface->UnlockRect();
}
