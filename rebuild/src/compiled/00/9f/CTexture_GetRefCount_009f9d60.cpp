// CTexture::GetRefCount() @ 0x009F9D60 (Ego_r/FableWin ?GetRefCount@CTexture@@QAEJXZ).
// COM has no reference-count query: AddRef, then Release returns the count.
#include "fable_surface_types.h"

long CTexture::GetRefCount()
{
    if (PD3DTexture)
    {
        PD3DTexture->AddRef();
        return PD3DTexture->Release();
    }
    return 0;
}
