#include "fable_volume_types.h"
#include "fable_memory_allocator.h"
unsigned long CVolumeTexture::Sizeof() const
{
    unsigned long size = 0;
    if (PD3DVolumeTexture)
    {
        if (PData) size = CMemoryAllocator::Get()->GetAllocationSize(PData);
        else size = ByteLength;
        size += 4;
    }
    return size;
}

unsigned long CVolumeTexture::TotalMemory;
void CVolumeTexture::ReleaseTexture()
{
    if (PD3DVolumeTexture)
    {
        PD3DVolumeTexture->AddRef();
        if (PD3DVolumeTexture->Release() == 2)
        {
            g_FableAllocatorMemoryBaseline_013CAAB0 -= Sizeof();
            TotalMemory -= Sizeof();
        }
        PD3DVolumeTexture->Release();
        PD3DVolumeTexture = 0;
    }
}

bool CVolumeTexture::InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture)
{
 Uninitialise();
 PD3DVolumeTexture=texture;
 CalcByteLength();
 TotalMemory+=Sizeof();
 unsigned long size=0;
 if (PD3DVolumeTexture)
 {
  if (PData) size=CMemoryAllocator::Get()->GetAllocationSize(PData)+4;
  else
  {
   g_FableAllocatorMemoryBaseline_013CAAB0+=ByteLength+4;
   return true;
  }
 }
 g_FableAllocatorMemoryBaseline_013CAAB0+=size;
 return true;
}
