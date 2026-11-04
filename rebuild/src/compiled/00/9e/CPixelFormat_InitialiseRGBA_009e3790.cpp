#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"

void CPixelFormat::Initialise(long r,long g,long b,long a,bool)
{
    long index=0;
    do {
        const CD3DFormatInfo& info=GD3DFormats[index];
        if(info.PFType==PF_TYPE_RGBA && info.RBits==r && info.GBits==g && info.BBits==b && info.AlphaBits==a) {
            FormatIndex=index;
            return;
        }
        ++index;
    } while(GD3DFormats[index].BitDepth!=-1);
}

