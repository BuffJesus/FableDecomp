#include "fable_texture_lifecycle.h"

void CTextureUninitialiseView::Uninitialise()
{
    if (texture00 != 0)
    {
        texture00->vtable->release08(texture00);
        texture00 = 0;
        flags04 &= 0x0FFFFFFF;
    }
}
