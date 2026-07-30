#include "fable_user_profile.h"

CUserProfileManager& FABLE_FASTCALL
CUserProfileManagerSingleton::Get()
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
