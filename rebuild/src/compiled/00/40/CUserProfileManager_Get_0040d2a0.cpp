// CUserProfileManager::Get, retail 0x0040D2A0 (FableWin ?Get@CUserProfileManager@@SIAAV1@XZ):
// the lazily created profile-manager singleton (CUserProfileManager::PClass, the
// counted pointer at 0x013B7D4C).
#include "fable_user_profile.h"

CUserProfileManager& CUserProfileManager::Get()
{
    CUserProfileManager* manager =
        g_CUserProfileManagerSingleton_013B7D4C.object;
    if (manager == 0)
    {
        g_CUserProfileManagerSingleton_013B7D4C =
            new CUserProfileManager;
    }

    return *g_CUserProfileManagerSingleton_013B7D4C.object;
}