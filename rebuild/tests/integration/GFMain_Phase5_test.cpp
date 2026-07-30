#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_gfmain.h"
#include "fable_string.h"

namespace
{
    const wchar_t* const kFullRetailPaths[] =
    {
        L"Data\\Graphics\\graphics.big",
        L"Data\\Graphics\\pc\\textures.big",
        L"Data\\lang\\English\\dialogue.big",
        L"Data\\Misc\\effects.big",
        L"Data\\lang\\English\\text.big",
        L"Data\\Misc\\temp.big",
        L"Data\\Shaders\\shaders.big",
        L"Data\\lang\\English\\fonts.big",
        L"Data\\lang\\English\\text.big"
    };

    const bool kFullRetailPriorities[] =
    {
        true, true, false, false, false, false, false, true, false
    };

    bool HasFullRetailSequence(const GFMainPhase5State& state)
    {
        if (state.openRetailBankCalls != 9)
            return false;

        for (fable_u32 index = 0; index < 9; ++index)
        {
            if (wcscmp(
                    state.retailBanks[index].pathname,
                    kFullRetailPaths[index]) != 0 ||
                state.retailBanks[index].startupPriority !=
                    kFullRetailPriorities[index])
                return false;
        }
        return true;
    }

    bool HasBalancedStrings(
        fable_i32 wideBaseline,
        fable_i32 charBaseline)
    {
        return g_CWideStringInstanceCount_013BCA20 == wideBaseline &&
            g_CCharStringInstanceCount_013BD800 == charBaseline;
    }
}

int main()
{
    const fable_i32 wideBaseline =
        g_CWideStringInstanceCount_013BCA20;
    const fable_i32 charBaseline =
        g_CCharStringInstanceCount_013BD800;
    GFMainPhase5State state = {};

    g_FableNavigatorEnabled_013B8616 = 1;
    g_FableOpenAllRetailBanks_013B8605 = 1;
    g_FableUseDvdBankList_013B8615 = 0;
    if (FableRunGFMainPhase5(state) != 0 ||
        !HasFullRetailSequence(state) ||
        state.openIniFileCalls != 0 ||
        state.setHeaderDirectoryCalls != 1 ||
        wcscmp(
            state.headerDirectory,
            L"data\\defs\\RetailHeaders\\") != 0 ||
        wcscmp(
            state.compiledDefinitionsDirectory,
            L"Data\\CompiledDefs\\") != 0 ||
        !HasBalancedStrings(wideBaseline, charBaseline))
    {
        printf("FABLETLC_GFMAIN_PHASE5_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    memset(&state, 0, sizeof(state));
    g_FableOpenAllRetailBanks_013B8605 = 0;
    if (FableRunGFMainPhase5(state) != 0 ||
        state.openRetailBankCalls != 2 ||
        wcscmp(
            state.retailBanks[0].pathname,
            L"Data\\lang\\English\\fonts.big") != 0 ||
        !state.retailBanks[0].startupPriority ||
        wcscmp(
            state.retailBanks[1].pathname,
            L"Data\\lang\\English\\text.big") != 0 ||
        state.retailBanks[1].startupPriority ||
        state.openIniFileCalls != 0 ||
        !HasBalancedStrings(wideBaseline, charBaseline))
    {
        printf("FABLETLC_GFMAIN_PHASE5_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    memset(&state, 0, sizeof(state));
    g_FableNavigatorEnabled_013B8616 = 0;
    g_FableUseDvdBankList_013B8615 = 1;
    if (FableRunGFMainPhase5(state) != 0 ||
        state.openRetailBankCalls != 0 ||
        state.openIniFileCalls != 1 ||
        wcscmp(state.iniPathname, L"banks_dvd.ini") != 0 ||
        state.setHeaderDirectoryCalls != 1 ||
        wcscmp(
            state.headerDirectory,
            L"data\\defs\\DevHeaders\\") != 0 ||
        wcscmp(
            state.compiledDefinitionsDirectory,
            L"Data\\CompiledDefs\\Development\\") != 0 ||
        !HasBalancedStrings(wideBaseline, charBaseline))
    {
        printf("FABLETLC_GFMAIN_PHASE5_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    memset(&state, 0, sizeof(state));
    g_FableUseDvdBankList_013B8615 = 0;
    if (FableRunGFMainPhase5(state) != 0 ||
        state.openIniFileCalls != 1 ||
        wcscmp(state.iniPathname, L"banks.ini") != 0 ||
        !HasBalancedStrings(wideBaseline, charBaseline))
    {
        printf("FABLETLC_GFMAIN_PHASE5_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    printf("FABLETLC_GFMAIN_PHASE5_BEHAVIOR PASS\n");
    return 0;
}
