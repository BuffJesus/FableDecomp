#include "fable_volume_types.h"
unsigned int CVolumeTexture::GetNoLevels() const
{
    return PD3DVolumeTexture->GetLevelCount();
}
