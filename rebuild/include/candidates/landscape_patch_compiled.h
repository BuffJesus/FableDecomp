#ifndef FABLETLC_LANDSCAPE_PATCH_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CMemoryDataInputStream
{
};

class CDataInputStream
{
public:
    static void ReadWithSrcChunkOverflow(CDataInputStream* stream, fable_u8* destination, int byteCount);
};

extern fable_u8* DAT_01436e98;
extern fable_u8* DAT_01436ea8;

class CEngineLandscapeRenderer
{
public:
    static void AddFrameForegroundTexturesUsed(
        fable_u8* renderer,
        fable_u16* textureIndices,
        fable_u32 textureCount,
        float distanceSquared);
};

class CEngineLandscapePatch
{
public:
    fable_u8 m_Pad00[0x0C];
    fable_u16* m_pForegroundTextureIndices;
    fable_u16 m_nForegroundTextureCount;
    fable_u8 m_Pad12[0x2A];

    fable_u8 m_Flags;

private:
    fable_u8 m_Pad3D[0x0B];

public:
    void LoadHeader(CMemoryDataInputStream& stream);
    bool LoadTextures(float distanceSquared);
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, m_pForegroundTextureIndices) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, m_nForegroundTextureCount) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, m_Flags) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch) == 0x48);

#endif
