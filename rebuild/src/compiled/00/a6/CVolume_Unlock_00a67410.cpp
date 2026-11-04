#include "fable_volume_types.h"
void CVolume::Unlock() const
{
    PD3DVolume->UnlockBox();
}
