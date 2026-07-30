#include "fable_gfmain.h"

#include <stddef.h>
#include <string.h>

// Authored parent coordinator for the ten recovered GFMain phase units.
// This is an integration seam, not a standalone retail-function parity claim.
long FABLE_FASTCALL FableRunGFMainComplete(
    void* instance,
    char* commandLine,
    int showCommand)
{
    long result = FableRunGFMainPhase1(commandLine);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase2(g_GFMainPhase2State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase3(g_GFMainPhase3State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase4(g_GFMainPhase4State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase5(g_GFMainPhase5State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase6(
        g_GFMainPhase5State,
        g_GFMainPhase6State);
    if (result != 0)
        return result;

    g_GFMainPhase7State.instanceHandleToken =
        static_cast<fable_u32>(reinterpret_cast<size_t>(instance));
    if (commandLine != 0)
    {
        strncpy(
            g_GFMainPhase7State.commandLine,
            commandLine,
            sizeof(g_GFMainPhase7State.commandLine) - 1);
        g_GFMainPhase7State.commandLine[
            sizeof(g_GFMainPhase7State.commandLine) - 1] = '\0';
    }
    g_GFMainPhase7State.windowsShow = showCommand;

    result = FableRunGFMainPhase7(g_GFMainPhase7State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase8(
        g_GFMainPhase7State,
        g_GFMainPhase8State);
    if (result != 0)
        return result;

    result = FableRunGFMainPhase9(g_GFMainPhase9State);
    if (result != 0)
        return result;

    return FableRunGFMainPhase10(
        g_GFMainPhase9State,
        g_GFMainPhase10State);
}
