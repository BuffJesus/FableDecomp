#ifndef FABLE_ENGINE_CENGINEDEFRETAIL_H
#define FABLE_ENGINE_CENGINEDEFRETAIL_H

// Retail fable.exe contracts the prefix preceding CEngineDef::SeaHeight by
// four bytes relative to the Ego_r PDB layout. Keep that executable-specific
// fact here instead of changing the readable donor definition.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
struct CEngineDefRetail {
    unsigned char _prefix[0x4c];
    float GamePrimitiveDefaultFadeStart;
    float GamePrimitiveDefaultFadeRangeRatio;
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(offsetof(CEngineDefRetail, GamePrimitiveDefaultFadeStart) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CEngineDefRetail, GamePrimitiveDefaultFadeRangeRatio) == 0x50);

#endif // FABLE_ENGINE_CENGINEDEFRETAIL_H
