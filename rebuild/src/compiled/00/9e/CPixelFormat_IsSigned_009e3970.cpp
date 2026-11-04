#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"

bool CPixelFormat::IsSigned() const
{
    if(GD3DFormats[FormatIndex].PFType==PF_TYPE_BUMP)return true;
    return false;
}

