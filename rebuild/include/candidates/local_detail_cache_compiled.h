#ifndef FABLETLC_LOCAL_DETAIL_CACHE_COMPILED_H
#define FABLETLC_LOCAL_DETAIL_CACHE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

#include "engine/CEngineStaticMapFileBlockManager.h"

class CDataInputStream
{
public:
    virtual void Slot00() = 0;
    virtual void SetSrcChunk(unsigned long chunk) = 0;
    long ReadPosition;
    fable_u8 pad_08[4];
    fable_u32* SrcCursor32;
    fable_u8 pad_10[4];
    long SrcBytesRemaining;

    static void ReadWithSrcChunkOverflow(CDataInputStream* stream, unsigned char* output, unsigned long bytes);
};

#include "engine/CLocalDetailCacheMap.h"

FABLE_STATIC_ASSERT(offsetof(CDataInputStream, ReadPosition) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SrcCursor32) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SrcBytesRemaining) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, StaticMapFileBlockManager) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, QuadTreeElement) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, ObjectTypeCollectionPalette) == 0x64);

#endif
