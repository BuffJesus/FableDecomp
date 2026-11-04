// lib_texture.cpp, retail 0x009FA0C0 / 0x009FA100 / 0x009FA140 / 0x009FA180
// (Ego_r CTexture::Swizzle / Unswizzle): forward to the level's surface.
#include "fable_surface_types.h"

// Retail 009FA0C0.
void CTexture::Swizzle(long level, long flags)
{
    GetSurfaceLevel(level).Swizzle(flags);
}

// Retail 009FA100.
void CTexture::Unswizzle(long level, long flags)
{
    GetSurfaceLevel(level).Unswizzle(flags);
}

// Retail 009FA140.
void CTexture::Swizzle(long level, CSurfaceLockedRect& lock)
{
    GetSurfaceLevel(level).Swizzle(lock);
}

// Retail 009FA180.
void CTexture::Unswizzle(long level, CSurfaceLockedRect& lock)
{
    GetSurfaceLevel(level).Unswizzle(lock);
}
