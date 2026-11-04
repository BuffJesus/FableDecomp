#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"

unsigned long CPixelFormat::GetByteColourDepth() const
{
    return GD3DFormats[FormatIndex].BitDepth / 8;
}

