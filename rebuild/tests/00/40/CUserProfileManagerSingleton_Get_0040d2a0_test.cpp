#include <stdio.h>

#include "fable_user_profile.h"

CCountedUserProfileManager
    g_CUserProfileManagerSingleton_013B7D4C = {0, 0};

namespace
{
    unsigned char g_storageBytes[sizeof(CUserProfileManager)];
    bool g_allocationEnabled = true;
    fable_u32 g_allocationCalls = 0;
    fable_u32 g_allocationSize = 0;
    fable_u32 g_constructorCalls = 0;
    fable_u32 g_assignmentCalls = 0;

    CUserProfileManager* Storage()
    {
        return reinterpret_cast<CUserProfileManager*>(
            g_storageBytes);
    }
}

void* FABLE_CDECL FableUserProfileManagerAllocate(fable_u32 size)
{
    ++g_allocationCalls;
    g_allocationSize = size;
    return g_allocationEnabled ? Storage() : 0;
}

CWideString::CWideString()
{
    *reinterpret_cast<CWideStringData**>(this) = 0;
}

CUserProfileManager::CUserProfileManager()
{
    ++g_constructorCalls;
}

void CCountedUserProfileManager::operator=(
    CUserProfileManager* value)
{
    ++g_assignmentCalls;
    object = value;
    reference = value != 0 ? this : 0;
}

static void ResetFixture(bool allocationEnabled)
{
    g_CUserProfileManagerSingleton_013B7D4C.object = 0;
    g_CUserProfileManagerSingleton_013B7D4C.reference = 0;
    g_allocationEnabled = allocationEnabled;
    g_allocationCalls = 0;
    g_allocationSize = 0;
    g_constructorCalls = 0;
    g_assignmentCalls = 0;
}

int main()
{
    ResetFixture(true);
    g_CUserProfileManagerSingleton_013B7D4C.object = Storage();
    CUserProfileManager* cached =
        &CUserProfileManagerSingleton::Get();
    if (
        cached != Storage() ||
        g_allocationCalls != 0 ||
        g_constructorCalls != 0 ||
        g_assignmentCalls != 0)
    {
        return 1;
    }

    ResetFixture(true);
    CUserProfileManager* created =
        &CUserProfileManagerSingleton::Get();
    if (
        created != Storage() ||
        g_allocationCalls != 1 ||
        g_allocationSize != sizeof(CUserProfileManager) ||
        g_constructorCalls != 1 ||
        g_assignmentCalls != 1 ||
        g_CUserProfileManagerSingleton_013B7D4C.reference == 0)
    {
        return 2;
    }

    ResetFixture(false);
    CUserProfileManager* unavailable =
        &CUserProfileManagerSingleton::Get();
    if (
        unavailable != 0 ||
        g_allocationCalls != 1 ||
        g_constructorCalls != 0 ||
        g_assignmentCalls != 1)
    {
        return 3;
    }

    puts("FABLETLC_USER_PROFILE_SINGLETON_GET_BEHAVIOR PASS");
    return 0;
}
