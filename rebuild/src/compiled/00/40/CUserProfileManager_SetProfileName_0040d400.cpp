#include "fable_user_profile.h"

void CUserProfileManager::SetProfileName(
    const CWideString& profileName,
    bool loadProfile)
{
    profileName_ = profileName;
    if (loadProfile)
        Load();
    else
        Save();
}
