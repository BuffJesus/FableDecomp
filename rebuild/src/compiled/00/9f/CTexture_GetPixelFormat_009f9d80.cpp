#include "fable_surface_types.h"
CPixelFormat CTexture::GetPixelFormat() const {
    D3DSURFACE_DESC description;
    PD3DTexture->GetLevelDesc(0,&description);
    return CPixelFormat(description.Format);
}
