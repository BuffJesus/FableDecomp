#include "fable_volume_types.h"
CVolumeTexture& CVolumeTexture::operator=(const CVolumeTexture& source)
{
 Uninitialise();
 PD3DVolumeTexture=source.PD3DVolumeTexture;
 ByteLength=source.ByteLength;
 PData=source.PData;
 if (source.PD3DVolumeTexture) PD3DVolumeTexture->AddRef();
 return *this;
}
