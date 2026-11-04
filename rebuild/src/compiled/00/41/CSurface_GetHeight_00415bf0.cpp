// CSurface::GetHeight (retail COMDAT 0x00415BF0, emitted out of line in the
// CMainGameComponent TU). FableWin.pdb ?GetHeight@CSurface@@QBEKXZ.
#include "fable_surface.h"

unsigned long CSurface::GetHeight() const
{
    return GetDimensions().Y;
}
