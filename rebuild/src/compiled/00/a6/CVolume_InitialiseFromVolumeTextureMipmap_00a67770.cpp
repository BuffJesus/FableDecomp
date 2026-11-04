#include "fable_volume_types.h"
#include "fable_memory_allocator.h"
unsigned long CVolume::TotalMemory;
void CVolume::InitialiseFromVolumeTextureMipmap(IDirect3DVolume9* volume)
{
    if (PD3DVolume)
    {
        PD3DVolume->Release();
        PD3DVolume = 0;
    }
    PD3DVolume = volume;
    volume->AddRef();
    unsigned long size = sizeof(*this);
    if (PD3DVolume)
        size = GetByteLength() + sizeof(*this) + sizeof(PD3DVolume);
    TotalMemory += size;
    size = sizeof(*this);
    if (PD3DVolume)
        size = GetByteLength() + sizeof(*this) + sizeof(PD3DVolume);
    g_FableAllocatorMemoryBaseline_013CAAB0 += size;
}
