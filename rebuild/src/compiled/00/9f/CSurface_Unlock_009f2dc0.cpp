#include "fable_surface_types.h"
void CSurface::Unlock() const
{
    PD3DSurface->UnlockRect();
}
