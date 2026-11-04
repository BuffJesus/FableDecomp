#include "fable_surface_types.h"
unsigned long CSurface::GetByteLength() const {
    D3DSURFACE_DESC dimensions;
    PD3DSurface->GetDesc(&dimensions);
    D3DSURFACE_DESC descriptor;
    PD3DSurface->GetDesc(&descriptor);
    CPixelFormat format(descriptor.Format);
    return format.GetColourDepth()*dimensions.Height*dimensions.Width/8;
}
