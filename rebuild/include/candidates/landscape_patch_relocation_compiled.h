#ifndef FABLETLC_LANDSCAPE_PATCH_RELOCATION_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_RELOCATION_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct CEngineLandscapePatchRelocationState
{
    fable_u8 Unknown00[0x5C];
    char RelocationBlocked;
};

// The retail ChangeLoadingPriority implementation ignores its nominal enum
// parameter, and the optimized RelocateData call site passes no value.
class CBankFileAsyncData
{
public:
    void ChangeLoadingPriority();
};

class CLandscapeLayerMesh
{
public:
    fable_u8 Unknown00[0x3C];
    CLandscapeLayerMesh* Next;

    void OnPostRelocation();
};

class CEngineLandscapePatch
{
public:
    fable_u8 Unknown00[4];
    CEngineLandscapePatchRelocationState* RelocationState;
    CLandscapeLayerMesh* LayerMeshes;
    CLandscapeLayerMesh* RelocatedLayerMesh;
    fable_u8 Unknown10[4];
    CBankFileAsyncData* AsyncData;

    bool CanRelocateData(long relocationType) const;
    void RelocateData(long relocationType, void* from, void* to, long size);
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatchRelocationState, RelocationBlocked) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, RelocationState) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, LayerMeshes) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, RelocatedLayerMesh) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, AsyncData) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CLandscapeLayerMesh, Next) == 0x3C);

#endif
