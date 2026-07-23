#ifndef FABLETLC_LANDSCAPE_RENDERER_RELEASE_COMPILED_H
#define FABLETLC_LANDSCAPE_RENDERER_RELEASE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CEngineLandscapeMap
{
public:
    void ReleaseDefaultPoolSurfaces();
};

struct CEngineLandscapeMapEntry
{
    CEngineLandscapeMap* LandscapeMap;
};

struct CEngineLandscapeMapList
{
    fable_u8 Unknown00[0x20];
    CEngineLandscapeMapEntry** Begin;
    CEngineLandscapeMapEntry** End;
};

extern CEngineLandscapeMapList* DAT_01436e8c;

class CEngineLandscapeRenderer
{
public:
    void OnReleaseDefaultPoolResources();
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMapEntry, LandscapeMap) == 0x00);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMapList, Begin) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeMapList, End) == 0x24);

#endif
