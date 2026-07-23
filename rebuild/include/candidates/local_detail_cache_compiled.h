#ifndef FABLETLC_LOCAL_DETAIL_CACHE_COMPILED_H
#define FABLETLC_LOCAL_DETAIL_CACHE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CEngineStaticMapFileBlockManager {};

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

class CQuadTreeElement
{
public:
    fable_u8 storage[0x58];
    void LoadHeader(CDataInputStream& stream);
};

class CObjectTypeCollectionPalette
{
public:
    void Load(CDataInputStream& stream);
};

class CLocalDetailCacheMap
{
public:
    fable_u8 pad_00[4];
    CEngineStaticMapFileBlockManager* StaticMapFileBlockManager;
    fable_u8 pad_08[4];
    CQuadTreeElement QuadTreeElement;
    CObjectTypeCollectionPalette ObjectTypeCollectionPalette;

    void OpenStaticMap(CEngineStaticMapFileBlockManager* manager, CDataInputStream& stream);
};

FABLE_STATIC_ASSERT(offsetof(CDataInputStream, ReadPosition) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SrcCursor32) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CDataInputStream, SrcBytesRemaining) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, StaticMapFileBlockManager) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, QuadTreeElement) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CLocalDetailCacheMap, ObjectTypeCollectionPalette) == 0x64);

#endif
