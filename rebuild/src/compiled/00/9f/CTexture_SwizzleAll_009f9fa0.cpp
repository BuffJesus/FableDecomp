// lib_texture.cpp, retail 0x009F9FA0 / 0x009FA030 (Ego_r CTexture::SwizzleAll /
// UnswizzleAll): every mip level's surface, with GetSurfaceLevel expanded
// (FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE) and the surface call out of line.
#define FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE
#include "fable_surface_types.h"

// Retail 009F9FA0.
void CTexture::SwizzleAll(long flags)
{
    long levels = PD3DTexture->GetLevelCount();
    for (long level = 0; level < levels; ++level)
        GetSurfaceLevel(level).Swizzle(flags);
}

// Retail 009FA030.
void CTexture::UnswizzleAll(long flags)
{
    long levels = PD3DTexture->GetLevelCount();
    for (long level = 0; level < levels; ++level)
        GetSurfaceLevel(level).Unswizzle(flags);
}
