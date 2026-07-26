#include "fable_async.h"
#include "fable_file_installer.h"
#include "fable_gfmain.h"
#include "fable_profile.h"
#include "fable_startup.h"
#include "fable_string.h"

namespace
{
    const char kSetupBasicInstallFilesProfile[] =
        "Setup Basic install files";
}

long FABLE_FASTCALL FableRunGFMainPhase2(GFMainPhase2State& state)
{
    NProfileTimer::EndProfile(0);

    {
        CCharString profile(kSetupBasicInstallFilesProfile, -1);
        NProfileTimer::StartProfile(profile, 0);
    }

    state.mirroredSystemSettingA = state.sharedSystemSetting;
    state.mirroredSystemSettingB = state.sharedSystemSetting;
    state.mirroredSystemSettingC = state.sharedSystemSetting;
    state.mirroredFileSystemSetting = state.fileSystemSetting;

    if (state.applyBasicInstallFlag &&
        !state.basicInstallAlreadyConfigured)
    {
        CFileInstaller& installer = CFileInstallerSingleton::Get();
        installer.basicInstallStartupFlag = state.basicInstallFlag;
    }

    if (state.clearStartupLatch)
        FableClearStartupLatch_013964A8();

    CBankFileAsync::SetEnableFailureHandling(
        state.enableAsyncFailureHandling);

    // Retail releases the temporary counted owner inline before entering
    // Phase 3. Reset keeps that ownership boundary explicit and testable.
    g_CFileInstallerSingleton_013CA818.Reset();
    return 0;
}
