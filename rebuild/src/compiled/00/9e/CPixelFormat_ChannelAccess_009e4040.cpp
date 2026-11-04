#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"
unsigned long CPixelFormat::GetGPosShift() const {return GD3DFormats[FormatIndex].BBits;}
unsigned long CPixelFormat::GetBPosShift() const {return 0;}
unsigned long CPixelFormat::GetNoRBits() const {return GD3DFormats[FormatIndex].RBits;}
unsigned long CPixelFormat::GetNoGBits() const {return GD3DFormats[FormatIndex].GBits;}
unsigned long CPixelFormat::GetNoBBits() const {return GD3DFormats[FormatIndex].BBits;}
