// CPixelFormat::GetColourDepth (retail 0x009E3820).
// FableWin.pdb ?GetColourDepth@CPixelFormat@@QBEKXZ.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

unsigned long CPixelFormat::GetColourDepth() const
{
    return GD3DFormats[FormatIndex].BitDepth;
}
