#ifndef FABLETLC_LANDSCAPE_RENDERER_BUILD_THEMES_COMPILED_H
#define FABLETLC_LANDSCAPE_RENDERER_BUILD_THEMES_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct CEngineThemeEntry
{
    fable_i32 AmbientTextureId;
    fable_i32 AmbientMaterialId;
    fable_i32 DetailTextureId;
    fable_i32 DetailMaterialId;
    fable_i32 CliffTextureId;
    fable_i32 CliffMaterialId;
    fable_i32 AmbientMipShift;
    fable_i32 DetailMipShift;
    fable_i32 CliffMipShift;
    fable_i32 FalloffMipShift;
    fable_i32 AmbientScale;
    fable_i32 DetailScale;
    fable_i32 FalloffTextureId;
    fable_i32 FalloffMaterialId;
};

struct CSoundThemeDefVTable
{
    void* Unused;
    void (*Destroy)(void* object);
};

struct CSoundThemeDefRuntime
{
    CSoundThemeDefVTable* VTable;
    fable_i32 ReferenceCount;
    fable_u8 Unknown08[0x34];
    fable_i32 AmbientTextureId;
    fable_i32 AmbientMaterialId;
    fable_i32 DetailTextureId;
    fable_i32 DetailMaterialId;
    fable_i32 CliffTextureId;
    fable_i32 CliffMaterialId;
    fable_i32 AmbientTiles;
    fable_i32 DetailTiles;
    fable_i32 CliffTiles;
    fable_i32 FalloffTiles;
    fable_i32 AmbientScale;
    fable_i32 DetailScale;
    fable_i32 FalloffMaterialId;
    fable_i32 FalloffTextureId;
};

struct CEngineThemeVector
{
    CEngineThemeEntry* Begin;
    CEngineThemeEntry* End;
    CEngineThemeEntry* Capacity;

    bool Resize(unsigned long count);
    unsigned long Size() const;
};

class CEngineLandscapeRenderer
{
public:
    CEngineThemeVector Themes;

    void BuildThemes();
};

long GetEngineThemeCount();
CSoundThemeDefRuntime* AcquireEngineThemeDefinition(unsigned long index);
fable_u16 GetEngineThemeTextureWidth(fable_i32 textureId);

FABLE_STATIC_ASSERT(sizeof(CEngineThemeEntry) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDefRuntime, AmbientTextureId) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDefRuntime, AmbientTiles) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDefRuntime, FalloffTextureId) == 0x70);
FABLE_STATIC_ASSERT(sizeof(CEngineThemeVector) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, Themes) == 0);

#endif
