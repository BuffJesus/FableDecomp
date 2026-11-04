#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"

void CSurface::SetPixel(long x,long y,const CRGBColour& colour,const CSurfaceLockedRect& rect)
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    CPixelFormat format(desc.Format);
    unsigned long offset=x*format.GetByteColourDepth();
    offset+=y*rect.Pitch;
    format.ConvertRGBToPFDData(colour,rect.PMem + offset);
}

