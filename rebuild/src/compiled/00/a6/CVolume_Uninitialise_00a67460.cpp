#include "fable_volume_types.h"
void CVolume::Uninitialise(bool)
{
    if (PD3DVolume)
    {
        PD3DVolume->Release();
        PD3DVolume = 0;
    }
}
