#ifndef FABLETLC_LANDSCAPE_PATCH_RELOCATION_COMPILED_H
#define FABLETLC_LANDSCAPE_PATCH_RELOCATION_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct CEngineLandscapePatchRelocationState
{
    fable_u8 Unknown00[0x5C];
    char RelocationBlocked;
};

class CEngineLandscapePatch
{
public:
    fable_u8 Unknown00[4];
    CEngineLandscapePatchRelocationState* RelocationState;

    bool CanRelocateData(long relocationType) const;
};

FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatchRelocationState, RelocationBlocked) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapePatch, RelocationState) == 0x04);

#endif
