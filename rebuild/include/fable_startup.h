#pragma once

#include "rebuild_abi.h"

// File-error hooks installed by GFMain. Retail's implementations are empty.
// Names and signatures confirmed by the FableWin startup call sites and PDB.
void FABLE_FASTCALL DoDirtyDisc();
void FABLE_FASTCALL GFHardDiskError(const char* directory);

// The PDB donor has no equivalent for this TLC-specific startup latch. Keep
// the retail address in the provisional name until its owning subsystem is
// identified from additional callers.
void FABLE_FASTCALL FableClearStartupLatch_013964A8();

extern bool g_FableStartupLatch_013964A8;
