#pragma once

#include "rebuild_abi.h"

// Phase 1 is an authored integration unit spanning retail GFMain
// 0x00402510-0x004025A6. It is not claimed as a standalone retail function.
long FABLE_FASTCALL FableRunGFMainPhase1(char* commandLine);

struct GFMainPhase2State
{
    bool sharedSystemSetting;
    bool mirroredSystemSettingA;
    bool mirroredSystemSettingB;
    bool mirroredSystemSettingC;

    bool fileSystemSetting;
    bool mirroredFileSystemSetting;

    bool applyBasicInstallFlag;
    bool basicInstallAlreadyConfigured;
    bool basicInstallFlag;

    bool clearStartupLatch;
    bool enableAsyncFailureHandling;
};

// Phase 2 is an authored integration unit spanning retail GFMain
// 0x004025A6-0x00402668. Its direct callees are retail-matched; this wrapper
// gives the sequence a readable state object while the enclosing GFMain body
// and its inline ownership cleanup remain under reconstruction.
long FABLE_FASTCALL FableRunGFMainPhase2(GFMainPhase2State& state);

// The 4,158-byte retail console-variable registrar remains behind this
// explicit boundary until its generated registration table is recovered.
void FABLE_FASTCALL FableInitialiseConsoleVariablesBoundary();

extern fable_u32 g_FableConsoleVariablesBoundaryCalls;
extern GFMainPhase2State g_GFMainPhase2State;

// Observable counters belong to the Stage 3 engine boundary, not retail.
// They let the integration fixture prove the counted singleton's lifetime.
extern fable_u32 g_FableFileInstallerConstructCalls;
extern fable_u32 g_FableFileInstallerDestroyCalls;
extern fable_u8 g_FableLastBasicInstallStartupFlag;
