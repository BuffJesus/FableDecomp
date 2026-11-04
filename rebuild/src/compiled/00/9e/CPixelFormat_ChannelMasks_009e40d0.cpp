#include "fable_surface_types.h"
#include "fable_d3d_format_info.h"
unsigned long CPixelFormat::GetGMask() const {return ((1UL << GD3DFormats[FormatIndex].GBits)-1) << GD3DFormats[FormatIndex].BBits;}
unsigned long CPixelFormat::GetBMask() const {return (1UL << GD3DFormats[FormatIndex].BBits)-1;}
unsigned long CPixelFormat::GetRScaleShift() const {return 8-GD3DFormats[FormatIndex].RBits;}
unsigned long CPixelFormat::GetGScaleShift() const {return 8-GD3DFormats[FormatIndex].GBits;}
unsigned long CPixelFormat::GetBScaleShift() const {return 8-GD3DFormats[FormatIndex].BBits;}
unsigned long CPixelFormat::GetAScaleShift() const {return 8-GD3DFormats[FormatIndex].AlphaBits;}
