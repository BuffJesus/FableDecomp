#ifndef FABLETLC_LANDSCAPE_MAP_OPEN_COMPILED_H
#define FABLETLC_LANDSCAPE_MAP_OPEN_COMPILED_H

#include <stddef.h>
#include "candidates/local_detail_cache_compiled.h"

class CReleaseTarget {};
typedef void (CReleaseTarget::*CReleaseMethod)();

struct CRefCountBlock
{
    long RefCount;
    CReleaseMethod Release;
    void* Object;
};

class CEngineStaticMapFileBlockManagerRef
{
public:
    void* Object;
    CRefCountBlock* RefCountBlock;
};

struct CMapFileHeader
{
    fable_u32 words[5];
};

class CEngineTexturePalette
{
public:
    fable_u8 storage[0x1C];
    void Load(CDataInputStream& stream);
};

class CEngineLandscapeMap
{
public:
    fable_u8 pad_00[0x18];
    void* BlockManagerObject;
    CRefCountBlock* BlockManagerRefCountBlock;
    CEngineStaticMapFileBlockManagerRef* StaticMapFileBlockManager;
    CMapFileHeader StaticMapHeader;
    fable_u8 pad_38[8];
    CEngineTexturePalette TexturePalette;

    void OpenStaticMap(CEngineStaticMapFileBlockManagerRef* manager, CDataInputStream& stream);
    void RelocateData(long relocationType, void* oldBase, void* newBase, long byteDelta);
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, BlockManagerObject) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CReleaseMethod) == 4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, StaticMapFileBlockManager) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, StaticMapHeader) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMap, TexturePalette) == 0x40);

#endif
