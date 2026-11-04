#include "fable_volume_types.h"
inline CVolume::CVolume(IDirect3DVolume9* volume) : PD3DVolume(0)
{
    InitialiseFromVolumeTextureMipmap(volume);
}
inline CVolume::~CVolume()
{
    Uninitialise(true);
}
CVolume CVolumeTexture::GetVolumeLevel(long level) const
{
    IDirect3DVolume9* volume;
    PD3DVolumeTexture->GetVolumeLevel(level, &volume);
    CVolume result(volume);
    return result;
}

#include <string.h>
#include "engine/CGraphicDataBank.h"
#include "fable_managed_texture.h"
template<> inline CCountedPointer<CVolumeTexture>::CCountedPointer(const CCountedPointer<CVolumeTexture>& other)
    : Data(other.Data), Info(other.Info)
{
    if (Info) ++Info->RefCount;
}
bool CGraphicDataBank::LoadVolumeGraphic(CManagedTexture& managed, CBankFileAsyncData& file,
    const CGraphicHeader&, bool, unsigned long, bool)
{
    CCountedPointer<CVolumeTexture> texture(managed.Data->VolumeTexture);
    CVolume volume = texture->GetVolumeLevel(0);
    volume.GetByteLength();
    CVolumeLockedBox box = volume.Lock(0);
    unsigned char* slice = box.PMem;
    unsigned char* data = (unsigned char*)file.GetData();
    unsigned long byteDepth = volume.GetPixelFormat().GetByteColourDepth();
    for (long z = 0; z < volume.GetDimensions().Z; ++z)
    {
        for (long y = 0; y < volume.GetDimensions().Y; ++y)
        {
            memcpy(slice + y * box.RowPitch, data, volume.GetDimensions().X * byteDepth);
            data += volume.GetDimensions().X * byteDepth;
        }
        slice += box.SlicePitch;
    }
    texture->GetVolumeLevel(0).Swizzle(box);
    volume.Unlock();
    return true;
}
