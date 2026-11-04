// CSurface::GetDimensions (retail 0x009F2F90).
// FableWin.pdb ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ.
#include "fable_surface.h"

C2DExtentsI CSurface::GetDimensions() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width, desc.Height);
}
