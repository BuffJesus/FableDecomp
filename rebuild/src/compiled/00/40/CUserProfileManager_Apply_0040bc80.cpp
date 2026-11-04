// CUserProfileManager::Apply, retail 0x0040BC80 (FableWin ?Apply@CUserProfileManager@@QAEXXZ):
// apply the profile values, and the video values too when the profile asks for it.
#include "fable_user_profile.h"

void CUserProfileManager::Apply()
{
    ApplyValues();
    if (ApplyVideoValuesOnLoad)
        ApplyVideoValues();
}