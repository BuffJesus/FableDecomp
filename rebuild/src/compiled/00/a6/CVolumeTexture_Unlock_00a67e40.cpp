#include "fable_volume_types.h"
void CVolumeTexture::Unlock(unsigned long level)
{
    PD3DVolumeTexture->UnlockBox(level);
}
