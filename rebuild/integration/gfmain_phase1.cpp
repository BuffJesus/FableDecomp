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

    CSystemManagerInit systemManagerInit;

    CWideString projectPath = CAFile::GetProjectPath();
    CAFile::SetCurrentPath(projectPath);

    // The registrar is kept visible as a boundary rather than replaced with
    // thousands of guessed calls. Phase 2 can proceed once this returns.
    FableInitialiseConsoleVariablesBoundary();
    return 0;
}
