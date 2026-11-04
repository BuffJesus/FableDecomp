#include "fable_volume_types.h"
CVolume::CVolume(const CVolume& source) : PD3DVolume(source.PD3DVolume)
{
    if (PD3DVolume)
        PD3DVolume->AddRef();
}
