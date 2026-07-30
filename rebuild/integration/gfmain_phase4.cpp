#include <string.h>

#include "fable_gfmain.h"
#include "fable_profile.h"
#include "fable_retail_banks.h"
#include "fable_string.h"

extern CBasicRetailBankManager
    g_FableBasicRetailBankManager_013CA79C;

namespace
{
    const char kSetupBasicRetailBanksProfile[] =
        "Setup basic retail banks";

    struct RetailBankAlias
    {
        const char* genericName;
        const char* platformName;
    };

    const RetailBankAlias kRetailBankAliases[] =
    {
        { "GBANK_MAIN", "GBANK_MAIN_PC" },
        { "GBANK_GUI", "GBANK_GUI_PC" },
        { "GBANK_FRONT_END", "GBANK_FRONT_END_PC" },
        { "PARTICLE_MAIN", "PARTICLE_MAIN_PC" },
        { "PARTICLE_FRONTEND", "PARTICLE_FRONTEND_PC" }
    };

    void CopyObservedName(char* destination, const char* source)
    {
        strncpy(destination, source, 31);
        destination[31] = '\0';
    }
}

long FABLE_FASTCALL FableRunGFMainPhase4(GFMainPhase4State& state)
{
    {
        CCharString profile(kSetupBasicRetailBanksProfile, -1);
        NProfileTimer::StartProfile(profile, 0);
    }

    g_FableBasicRetailBankManager_013CA79C.SetNavigatorEnabled(
        g_FableNavigatorEnabled_013B8616 != 0);
    ++state.setNavigatorCalls;
    state.navigatorEnabled =
        g_FableBasicRetailBankManager_013CA79C.navigatorEnabled_;

    for (fable_u32 index = 0;
         index < sizeof(kRetailBankAliases) /
            sizeof(kRetailBankAliases[0]);
         ++index)
    {
        const RetailBankAlias& alias = kRetailBankAliases[index];
        CCharString genericName(alias.genericName, -1);
        CCharString platformName(alias.platformName, -1);

        g_FableBasicRetailBankManager_013CA79C.AddBankAlias(
            genericName,
            platformName);

        CopyObservedName(
            state.aliases[index].platformName,
            alias.platformName);
        CopyObservedName(
            state.aliases[index].genericName,
            alias.genericName);
        ++state.addAliasCalls;
    }

    return 0;
}
