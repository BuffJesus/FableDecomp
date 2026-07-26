#include <stdio.h>

#include "fable_async.h"
#include "fable_gfmain.h"
#include "fable_startup.h"

int main()
{
    GFMainPhase2State state = {};
    state.sharedSystemSetting = true;
    state.fileSystemSetting = true;
    state.applyBasicInstallFlag = true;
    state.basicInstallAlreadyConfigured = false;
    state.basicInstallFlag = true;
    state.clearStartupLatch = true;
    state.enableAsyncFailureHandling = true;

    g_FableStartupLatch_013964A8 = true;
    g_FableFileInstallerConstructCalls = 0;
    g_FableFileInstallerDestroyCalls = 0;
    g_FableLastBasicInstallStartupFlag = 0;

    if (FableRunGFMainPhase2(state) != 0)
    {
        printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (!state.mirroredSystemSettingA ||
        !state.mirroredSystemSettingB ||
        !state.mirroredSystemSettingC ||
        !state.mirroredFileSystemSetting)
    {
        printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR FAIL code=2\n");
        return 2;
    }
    if (g_FableStartupLatch_013964A8 ||
        g_CBankFileAsyncFailureHandlingMode_013CA868 != 0)
    {
        printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR FAIL code=3\n");
        return 3;
    }
    if (g_FableFileInstallerConstructCalls != 1 ||
        g_FableFileInstallerDestroyCalls != 1 ||
        g_FableLastBasicInstallStartupFlag != 1)
    {
        printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    state.applyBasicInstallFlag = false;
    state.clearStartupLatch = false;
    state.enableAsyncFailureHandling = false;
    g_FableStartupLatch_013964A8 = true;
    g_FableFileInstallerConstructCalls = 0;
    g_FableFileInstallerDestroyCalls = 0;

    if (FableRunGFMainPhase2(state) != 0 ||
        !g_FableStartupLatch_013964A8 ||
        g_CBankFileAsyncFailureHandlingMode_013CA868 != 2 ||
        g_FableFileInstallerConstructCalls != 0 ||
        g_FableFileInstallerDestroyCalls != 0)
    {
        printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR FAIL code=5\n");
        return 5;
    }

    printf("FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS\n");
    return 0;
}
