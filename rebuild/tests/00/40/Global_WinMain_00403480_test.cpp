#include <stdio.h>

#include "fable_boot.h"

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

namespace
{
    int g_setStackCalls = 0;
    int g_gfMainCalls = 0;
    fable_u8* g_observedStackTop = 0;
    fable_u32 g_observedStackSize = 0;
    FableInstanceHandle g_observedInstance = 0;
    char* g_observedCommandLine = 0;
    int g_observedShowCommand = 0;
}

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(fable_u8* stackTop, fable_u32 stackSize)
    {
        ++g_setStackCalls;
        g_observedStackTop = stackTop;
        g_observedStackSize = stackSize;
    }
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
    ++g_gfMainCalls;
    g_observedInstance = instance;
    g_observedCommandLine = commandLine;
    g_observedShowCommand = showCommand;
    return 0;
}

static int VerifyFirstInstancePath()
{
    char commandLine[] = "-stage1-fixture";
    FableInstanceHandle instance =
        reinterpret_cast<FableInstanceHandle>(0x12345678UL);

    if (WinMain(instance, 0, commandLine, 7) != 0)
        return 1;
    if (g_setStackCalls != 1 || g_gfMainCalls != 1)
        return 2;
    if (g_observedStackSize != 0x00032000UL)
        return 3;
    if (g_observedStackTop != g_FableMicroThreadStackTop_013B7C84)
        return 4;
    if (g_FableMicroThreadStackBottom_013B7C88 == 0)
        return 5;
    if (g_observedStackTop - g_FableMicroThreadStackBottom_013B7C88
        != 0x00032001UL)
        return 6;
    if (g_observedInstance != instance)
        return 7;
    if (g_observedCommandLine != commandLine)
        return 8;
    if (g_observedShowCommand != 7)
        return 9;

    return 0;
}

static int VerifyExistingInstancePath()
{
    const int setStackCalls = g_setStackCalls;
    const int gfMainCalls = g_gfMainCalls;
    char commandLine[] = "-second-instance";

    if (WinMain(0, 0, commandLine, 3) != 0)
        return 10;
    if (g_setStackCalls != setStackCalls)
        return 11;
    if (g_gfMainCalls != gfMainCalls)
        return 12;

    return 0;
}

int main()
{
    int result = VerifyFirstInstancePath();
    if (result == 0)
        result = VerifyExistingInstancePath();

    if (result != 0)
    {
        printf("FABLETLC_WINMAIN_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_WINMAIN_BEHAVIOR PASS\n");
    return 0;
}
