#include "fable_volume_types.h"
void CVolume::ReleaseVolume()
{
    if (PD3DVolume)
    {
        PD3DVolume->Release();
        PD3DVolume = 0;
    }
}
