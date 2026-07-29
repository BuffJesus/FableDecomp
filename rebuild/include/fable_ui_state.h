#pragma once

#include "rebuild_abi.h"

#include <stddef.h>

// Retail NUISystem::CUIState is 0x24 bytes. Keep this ABI view separate from
// the later C++23 ownership model: it is the differential reference layout
// recovered from the retail executable and both PDB-bearing donors.
struct FableUiStateVector2
{
    float x;
    float y;
};

struct FableUiStateColour
{
    fable_u8 red;
    fable_u8 green;
    fable_u8 blue;
    fable_u8 alpha;
};

struct CUIStateRecoveredLayout
{
    fable_u32 stateChangeFlag;
    fable_i32 stateChangeType;
    FableUiStateVector2 position;
    FableUiStateVector2 zoom;
    FableUiStateColour colour;
    float updateTime;
    fable_u8 linearChange;
    fable_u8 padding21[3];
};

FABLE_STATIC_ASSERT(sizeof(CUIStateRecoveredLayout) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CUIStateRecoveredLayout, position) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CUIStateRecoveredLayout, zoom) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CUIStateRecoveredLayout, colour) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CUIStateRecoveredLayout, updateTime) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CUIStateRecoveredLayout, linearChange) == 0x20);
