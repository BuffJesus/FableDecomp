#pragma once

#include "rebuild_abi.h"

// Phase 1 is an authored integration unit spanning retail GFMain
// 0x00402510-0x004025A6. It is not claimed as a standalone retail function.
long FABLE_FASTCALL FableRunGFMainPhase1(char* commandLine);

// The 4,158-byte retail console-variable registrar remains behind this
// explicit boundary until its generated registration table is recovered.
void FABLE_FASTCALL FableInitialiseConsoleVariablesBoundary();

extern fable_u32 g_FableConsoleVariablesBoundaryCalls;
