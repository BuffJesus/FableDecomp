#include "fable_filesystem.h"
#include "fable_gfmain.h"
#include "fable_profile.h"
#include "fable_system.h"

namespace
{
    const char kParseCommandLineProfile[] = "Parse Command Line";
}

long FABLE_FASTCALL FableRunGFMainPhase1(char* /* commandLine */)
{
    CCharString profile(
        kParseCommandLineProfile,
        sizeof(kParseCommandLineProfile) - 1);
    NProfileTimer::StartProfile(profile, 0);

    // Retail keeps this owner alive through Phase 9 system initialization.
    // The authored accessor gives the split phase units that shared lifetime.
    FableGetSystemManagerInitBoundary();

    CWideString projectPath = CAFile::GetProjectPath();
    CAFile::SetCurrentPath(projectPath);

    // The exact 4,158-byte registrar is parity/behavior-gated independently.
    // Stage 2 keeps its execution boundary until the recovered console object
    // model is linked into the authored runtime.
    FableInitialiseConsoleVariablesBoundary();
    return 0;
}
