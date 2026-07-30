#include <stdio.h>

#include "rebuild_abi.h"

extern "C" void FABLE_CDECL FableExit_00401284(int);

namespace
{
    fable_u32 g_calls;
    int g_observedExitCode;

    void FABLE_CDECL TestExit(int exitCode)
    {
        ++g_calls;
        g_observedExitCode = exitCode;
    }
}

extern "C" void (FABLE_CDECL* g_FableCrtExit_014401A0)(int) = &TestExit;

int main()
{
    FableExit_00401284(-17);
    const bool passed = g_calls == 1 && g_observedExitCode == -17;
    if (!passed)
    {
        printf("FABLETLC_EXIT_THUNK_BEHAVIOR FAIL\n");
        return 1;
    }
    printf("FABLETLC_EXIT_THUNK_BEHAVIOR PASS\n");
    return 0;
}
