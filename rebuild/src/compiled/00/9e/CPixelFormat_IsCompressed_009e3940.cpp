#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"
bool CPixelFormat::IsCompressed() const
{
 switch(GD3DFormats[FormatIndex].PFType)
 {
 case PF_TYPE_COMPRESSED_ALPHA:
 case PF_TYPE_COMPRESSED_BOOLEAN_ALPHA:
 case PF_TYPE_COMPRESSED_INTERPOLATED_ALPHA:
  return true;
 default:
  return false;
 }
}
