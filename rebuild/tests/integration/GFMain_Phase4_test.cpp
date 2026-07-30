#include <stdio.h>
#include <string.h>

#include "fable_gfmain.h"
#include "fable_string.h"

namespace
{
    const char* const kExpectedPlatformNames[] =
    {
        "GBANK_MAIN_PC",
        "GBANK_GUI_PC",
        "GBANK_FRONT_END_PC",
        "PARTICLE_MAIN_PC",
        "PARTICLE_FRONTEND_PC"
    };

    const char* const kExpectedGenericNames[] =
    {
        "GBANK_MAIN",
        "GBANK_GUI",
        "GBANK_FRONT_END",
        "PARTICLE_MAIN",
        "PARTICLE_FRONTEND"
    };

    bool HasExpectedAliases(const GFMainPhase4State& state)
    {
        for (fable_u32 index = 0; index < 5; ++index)
        {
            if (strcmp(
                    state.aliases[index].platformName,
                    kExpectedPlatformNames[index]) != 0 ||
                strcmp(
                    state.aliases[index].genericName,
                    kExpectedGenericNames[index]) != 0)
                return false;
        }
        return true;
    }
}

int main()
{
    const fable_i32 charStringBaseline =
        g_CCharStringInstanceCount_013BD800;
    GFMainPhase4State state = {};

    g_FableNavigatorEnabled_013B8616 = 1;
    if (FableRunGFMainPhase4(state) != 0 ||
        state.setNavigatorCalls != 1 ||
        state.addAliasCalls != 5 ||
        !state.navigatorEnabled ||
        !HasExpectedAliases(state) ||
        g_CCharStringInstanceCount_013BD800 != charStringBaseline)
    {
        printf("FABLETLC_GFMAIN_PHASE4_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    memset(&state, 0, sizeof(state));
    g_FableNavigatorEnabled_013B8616 = 0;
    if (FableRunGFMainPhase4(state) != 0 ||
        state.setNavigatorCalls != 1 ||
        state.addAliasCalls != 5 ||
        state.navigatorEnabled ||
        !HasExpectedAliases(state) ||
        g_CCharStringInstanceCount_013BD800 != charStringBaseline)
    {
        printf("FABLETLC_GFMAIN_PHASE4_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_GFMAIN_PHASE4_BEHAVIOR PASS\n");
    return 0;
}
