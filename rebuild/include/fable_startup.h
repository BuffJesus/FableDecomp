#pragma once

#include "rebuild_abi.h"

// The PDB donor has no equivalent for this TLC-specific startup latch. Keep
// the retail address in the provisional name until its owning subsystem is
// identified from additional callers.
void FABLE_FASTCALL FableClearStartupLatch_013964A8();

extern bool g_FableStartupLatch_013964A8;
