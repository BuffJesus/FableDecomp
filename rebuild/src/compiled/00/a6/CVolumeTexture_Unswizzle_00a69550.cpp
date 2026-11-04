#include "fable_volume_types.h"
inline CVolume::~CVolume() { Uninitialise(true); }
void CVolumeTexture::Unswizzle(unsigned int level, CVolumeLockedBox& box)
{
 GetVolumeLevel((long)level).Unswizzle(box);
}
