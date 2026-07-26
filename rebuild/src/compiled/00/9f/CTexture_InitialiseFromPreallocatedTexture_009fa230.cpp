#include "fable_texture_lifecycle.h"

bool CTexturePreallocatedView::InitialiseFromPreallocatedTexture(
    FableLifecycleTexture* texture)
{
    if (texture00 != 0)
    {
        texture00->vtable->release08(texture00);
        texture00 = 0;
        flags04 &= 0x0FFFFFFF;
    }

    texture00 = texture;
    const fable_u32 hasTexture =
        static_cast<fable_u32>(texture != 0);
    flags04 =
        (flags04 & 0x0FFFFFFF) |
        (hasTexture << 28);
    CalcByteLength();
    return true;
}
