#include <stdio.h>
#include <string.h>

#include "fable_gfmain.h"

GFMainPhase2State g_GFMainPhase2State = {};
GFMainPhase3State g_GFMainPhase3State = {};
GFMainPhase4State g_GFMainPhase4State = {};
GFMainPhase5State g_GFMainPhase5State = {};
GFMainPhase6State g_GFMainPhase6State = {};
GFMainPhase7State g_GFMainPhase7State = {};
GFMainPhase8State g_GFMainPhase8State = {};
GFMainPhase9State g_GFMainPhase9State = {};
GFMainPhase10State g_GFMainPhase10State = {};

namespace
{
    int g_failPhase;
    int g_callCount;
    int g_calls[10];

    long ObservePhase(int phase)
    {
        g_calls[g_callCount++] = phase;
        return g_failPhase == phase ? 100 + phase : 0;
    }

    void ResetScenario(int failPhase)
    {
        g_failPhase = failPhase;
        g_callCount = 0;
        memset(g_calls, 0, sizeof(g_calls));
        memset(&g_GFMainPhase2State, 0, sizeof(g_GFMainPhase2State));
        memset(&g_GFMainPhase3State, 0, sizeof(g_GFMainPhase3State));
        memset(&g_GFMainPhase4State, 0, sizeof(g_GFMainPhase4State));
        memset(&g_GFMainPhase5State, 0, sizeof(g_GFMainPhase5State));
        memset(&g_GFMainPhase6State, 0, sizeof(g_GFMainPhase6State));
        memset(&g_GFMainPhase7State, 0, sizeof(g_GFMainPhase7State));
        memset(&g_GFMainPhase8State, 0, sizeof(g_GFMainPhase8State));
        memset(&g_GFMainPhase9State, 0, sizeof(g_GFMainPhase9State));
        memset(&g_GFMainPhase10State, 0, sizeof(g_GFMainPhase10State));
    }

    bool CallsAreSequential(int count)
    {
        if (g_callCount != count)
            return false;
        for (int index = 0; index < count; ++index)
        {
            if (g_calls[index] != index + 1)
                return false;
        }
        return true;
    }
}

long FABLE_FASTCALL FableRunGFMainPhase1(char*)
{
    return ObservePhase(1);
}

long FABLE_FASTCALL FableRunGFMainPhase2(GFMainPhase2State&)
{
    return ObservePhase(2);
}

long FABLE_FASTCALL FableRunGFMainPhase3(GFMainPhase3State&)
{
    return ObservePhase(3);
}

long FABLE_FASTCALL FableRunGFMainPhase4(GFMainPhase4State&)
{
    return ObservePhase(4);
}

long FABLE_FASTCALL FableRunGFMainPhase5(GFMainPhase5State&)
{
    return ObservePhase(5);
}

long FABLE_FASTCALL FableRunGFMainPhase6(
    const GFMainPhase5State&,
    GFMainPhase6State&)
{
    return ObservePhase(6);
}

long FABLE_FASTCALL FableRunGFMainPhase7(GFMainPhase7State&)
{
    return ObservePhase(7);
}

long FABLE_FASTCALL FableRunGFMainPhase8(
    const GFMainPhase7State&,
    GFMainPhase8State&)
{
    return ObservePhase(8);
}

long FABLE_FASTCALL FableRunGFMainPhase9(GFMainPhase9State&)
{
    return ObservePhase(9);
}

long FABLE_FASTCALL FableRunGFMainPhase10(
    const GFMainPhase9State&,
    GFMainPhase10State&)
{
    return ObservePhase(10);
}

int main()
{
    char commandLine[] = "windowed -dev";
    void* const instance = reinterpret_cast<void*>(0x12345678UL);

    ResetScenario(0);
    if (FableRunGFMainComplete(instance, commandLine, 7) != 0)
        return 1;
    if (!CallsAreSequential(10))
        return 2;
    if (g_GFMainPhase7State.instanceHandleToken != 0x12345678UL ||
        strcmp(g_GFMainPhase7State.commandLine, commandLine) != 0 ||
        g_GFMainPhase7State.windowsShow != 7)
        return 3;

    for (int phase = 1; phase <= 10; ++phase)
    {
        ResetScenario(phase);
        if (FableRunGFMainComplete(instance, commandLine, 7) != 100 + phase)
            return 10 + phase;
        if (!CallsAreSequential(phase))
            return 20 + phase;
    }

    char longCommandLine[160];
    memset(longCommandLine, 'X', sizeof(longCommandLine));
    longCommandLine[sizeof(longCommandLine) - 1] = '\0';
    ResetScenario(0);
    if (FableRunGFMainComplete(instance, longCommandLine, 1) != 0)
        return 40;
    if (strlen(g_GFMainPhase7State.commandLine) !=
        sizeof(g_GFMainPhase7State.commandLine) - 1)
        return 41;

    ResetScenario(0);
    strcpy(g_GFMainPhase7State.commandLine, "preserved");
    if (FableRunGFMainComplete(0, 0, 0) != 0)
        return 42;
    if (strcmp(g_GFMainPhase7State.commandLine, "preserved") != 0)
        return 43;

    puts("FABLETLC_GFMAIN_COMPLETE PASS");
    return 0;
}
