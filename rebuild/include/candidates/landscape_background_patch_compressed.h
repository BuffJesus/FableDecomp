#ifndef FABLETLC_LANDSCAPE_BACKGROUND_PATCH_COMPRESSED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_PATCH_COMPRESSED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CMemoryDataInputStream
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual long GetPosition();

    long ReadPosition;
    fable_u32 Unknown08;
    unsigned char* Cursor;
    fable_u32 Unknown10;
    long RemainingInSourceChunk;
    fable_i32 SourceChunkBase;

    CMemoryDataInputStream(unsigned char* data, unsigned long size);
    ~CMemoryDataInputStream();

    static void ReadWithSrcChunkOverflow(
        CMemoryDataInputStream* stream,
        unsigned char* destination,
        unsigned long size);
    void SetPositionAssumeWithinSourceChunk(long position);
};

FABLE_STATIC_ASSERT(sizeof(CMemoryDataInputStream) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, ReadPosition) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, Cursor) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, RemainingInSourceChunk) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CMemoryDataInputStream, SourceChunkBase) == 0x18);

class CTemporaryBuffer
{
public:
    fable_u32 Unknown00;
    unsigned char* Data;
    fable_u8 Unknown08[0x10];

    CTemporaryBuffer(unsigned long size, void* memoryPool, void* allocationTag);
    ~CTemporaryBuffer();
};

FABLE_STATIC_ASSERT(sizeof(CTemporaryBuffer) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTemporaryBuffer, Data) == 0x04);

extern fable_u8* DAT_01436ea8;
extern fable_u8 DAT_013cb498;

void LZSS_Decompress(
    fable_i32 source,
    fable_i32 compressedSize,
    unsigned char* destination,
    unsigned long* destinationSize,
    fable_i32 flags);

class CLandscapeBackgroundPatch
{
public:
    bool LoadCompressed(CMemoryDataInputStream& stream, float worldX, float worldY);
    bool Load(CMemoryDataInputStream& stream, float worldX, float worldY);
};

#endif
