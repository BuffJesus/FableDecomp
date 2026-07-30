#include <stdio.h>
#include <string.h>

#include "fable_user_profile.h"

namespace
{
    fable_u32 g_loadCalls = 0;
    fable_u32 g_saveCalls = 0;
    CWideString* g_assignmentDestination = 0;
    const CWideString* g_assignmentSource = 0;

    struct UserProfileManagerView
    {
        void* primaryVTable;
        void* secondaryVTable;
        CWideStringData* profileNameStorage;
    };
}

CWideString& CWideString::operator=(const CWideString& other)
{
    g_assignmentDestination = this;
    g_assignmentSource = &other;
    *reinterpret_cast<CWideStringData**>(this) =
        *reinterpret_cast<CWideStringData* const*>(&other);
    return *this;
}

void CUserProfileManager::Load()
{
    ++g_loadCalls;
}

void CUserProfileManager::Save()
{
    ++g_saveCalls;
}

int main()
{
    unsigned char managerStorage[sizeof(CUserProfileManager)];
    memset(managerStorage, 0, sizeof(managerStorage));
    CUserProfileManager& manager =
        *reinterpret_cast<CUserProfileManager*>(managerStorage);
    UserProfileManagerView& view =
        *reinterpret_cast<UserProfileManagerView*>(managerStorage);

    CWideStringData profileStorage = {};
    CWideStringData* profileSlot = &profileStorage;
    const CWideString& profileName =
        *reinterpret_cast<const CWideString*>(&profileSlot);

    manager.SetProfileName(profileName, true);
    if (
        g_loadCalls != 1 ||
        g_saveCalls != 0 ||
        g_assignmentDestination !=
            reinterpret_cast<CWideString*>(&view.profileNameStorage) ||
        g_assignmentSource != &profileName ||
        view.profileNameStorage != &profileStorage)
    {
        return 1;
    }

    manager.SetProfileName(profileName, false);
    if (g_loadCalls != 1 || g_saveCalls != 1)
        return 2;

    puts("FABLETLC_USER_PROFILE_SET_PROFILE_NAME_BEHAVIOR PASS");
    return 0;
}
