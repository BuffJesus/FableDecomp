// CTexture::Unlock(unsigned long) @ 0x009F9DE0 (Ego_r/FableWin ?Unlock@CTexture@@QAEXK@Z).
// Unlocks one mip level of the Direct3D texture (IDirect3DTexture9::UnlockRect, vtable +0x50).
#include "fable_surface_types.h"

void CTexture::Unlock(unsigned long level)
{
    PD3DTexture->UnlockRect(level);
}
