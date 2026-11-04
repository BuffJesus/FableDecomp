#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_carray.h"
#include "fable_surface_types.h"
#include "fable_display_viewport.h"
// Donor member identities reconciled against retail F36F40/F37030. Retail
// CArrays are12bytes, shifting the manager/page tails from the donor layout.
class CGPUScratchBufferTextureManager : public CBaseClassNonCopyable {
public:
    enum EResourceType { RESOURCE_TYPE_NOT_ALLOCATED,RESOURCE_TYPE_TEXTURE,RESOURCE_TYPE_DEPTH_BUFFER_SURFACE };
    struct CResourceEntry {
        EResourceType ResourceType;
        CTexture* Texture;
        CSurface* Surface;
        long PageIndex;
        C2DExtentsI Dimensions;
        long NoLevels;
        CPixelFormat PixelFormat;
        ESurfaceMultisampleType MultiSampleType;
        unsigned long UseageFlags;
        ESurfacePool MemoryPool;
    };
    struct CResourcePage {
        long ScratchPool;
        CArrayVector<long> ResourceList;
        unsigned char* NextFreeBufferAddress;
        long FreeSpaceSize;
        bool EnableAutoRestore;
    };
    struct CScratchPool {unsigned char* Buffer;long BufferSize;};
    void FreeMemoryPage(long);
    CArrayVector<CResourceEntry> Resources;
    CArrayVector<CResourcePage> MemoryPages;
    CArrayVector<CScratchPool> ScratchPools;
    CArrayVector<long> FreeTexturePageSlots;
    CArrayVector<long> FreeTextureSlots;
    bool ShutdownFlag;
private:
    void FreeResourceEntry(long);
};
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager::CResourceEntry)==0x2c);
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager::CResourcePage)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CGPUScratchBufferTextureManager,MemoryPages)==0x10);
FABLE_STATIC_ASSERT(offsetof(CGPUScratchBufferTextureManager,FreeTexturePageSlots)==0x28);
FABLE_STATIC_ASSERT(offsetof(CGPUScratchBufferTextureManager,FreeTextureSlots)==0x34);
FABLE_STATIC_ASSERT(offsetof(CGPUScratchBufferTextureManager,ShutdownFlag)==0x40);
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager)==0x44);
