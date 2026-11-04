#include "fable_volume_types.h"
CVolumeTexture::CVolumeTexture(const CVolumeTexture& source)
 : PD3DVolumeTexture(source.PD3DVolumeTexture), ByteLength(source.ByteLength), PData(source.PData)
{
 if (source.PD3DVolumeTexture) PD3DVolumeTexture->AddRef();
}
