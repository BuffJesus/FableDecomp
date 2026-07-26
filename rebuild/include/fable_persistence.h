#pragma once

#include "rebuild_abi.h"

struct CBase
{
    void** vtable;
};

extern void* g_CBaseVTable_01231710[];

// These two folded Phase 3 cleanup leaves currently have only the PDB's
// generic CBase identity. Address-bearing names avoid pretending we know
// which derived persistence objects own each call site.
void FABLE_FASTCALL FableRestoreCBaseVTable_0099A2E0(CBase* object);
void FABLE_FASTCALL FableRestoreCBaseVTable_0099A300(CBase* object);
