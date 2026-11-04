#include "fable_volume_types.h"
inline CVolume::~CVolume() { Uninitialise(true); }
void CVolumeTexture::CalcByteLength()
{
 ByteLength=0;
 if (PD3DVolumeTexture)
 {
  long levels=GetNoLevels();
  for (long level=0;level<levels;++level)
  {
   CVolume volume=GetVolumeLevel(level);
   ByteLength+=volume.GetByteLength();
  }
 }
}
