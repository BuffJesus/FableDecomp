#include "fable_volume_types.h"
inline CVolume::~CVolume() { Uninitialise(true); }
void CVolumeTexture::CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter)
{
 long sourceCount=source.PD3DVolumeTexture->GetLevelCount();
 long destinationCount=PD3DVolumeTexture->GetLevelCount();
 const long& minimum=sourceCount<destinationCount?sourceCount:destinationCount;
 long count=minimum;
 for(long level=0;level<count;++level)
 {
  GetVolumeLevel(level).CopyFromVolume(0,source.GetVolumeLevel(level),0,filter);
 }
}
