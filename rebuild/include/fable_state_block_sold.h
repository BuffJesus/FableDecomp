#pragma once

#include "rebuild_abi.h"

struct CTrackedDwordStateSoldView
{
    fable_u32 realisedValue00;
    fable_u32 requestedValue04;
    fable_u32 captureMask08;
    fable_u32 deviceState0C;
    fable_u8 queuedForRealise10;
    fable_u8 stateFamily11;
    fable_u8 stage12;
    fable_u8 reserved13;
};

struct CCapturedDwordStateSoldView
{
    CTrackedDwordStateSoldView* state00;
    fable_u32 priorRequestedValue04;
};

struct CStateBlockFunctionSoldView
{
    // Retail 0x009DF060. The body resolves the state manager through
    // GFGetSystemManager(): system +0x60 -> render system +0x08.
    void Apply();
};

// Compact integrations provide this object-graph root and the eight retail
// D3D value globals. Apply queues 17 states in the manager's capture/update
// arrays; RealiseRenderState remains the later device-dispatch step.
struct CSystemManager;
extern CSystemManager* FABLE_FASTCALL GFGetSystemManager();

extern fable_u32 DAT_01396f6c;
extern fable_u32 DAT_01396f68;
extern fable_u32 DAT_01396e04;
extern fable_u32 DAT_01396ef0;
extern fable_u32 DAT_01396eec;
extern fable_u32 DAT_01396e7c;
extern fable_u32 DAT_01396e70;
extern fable_u32 DAT_01396e5c;

FABLE_STATIC_ASSERT(sizeof(CTrackedDwordStateSoldView) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CCapturedDwordStateSoldView) == 0x08);
