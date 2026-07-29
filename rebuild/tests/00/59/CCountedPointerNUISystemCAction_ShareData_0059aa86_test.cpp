#include <cstdio>

#include "../../../src/compiled/00/59/CCountedPointerNUISystemCAction_ShareData_0059aa86.cpp"

static int g_ReleaseCalls;
static void* g_ActionSeenAtRelease;
static FrontEndActionPointerInfo_0059aa86* g_InfoSeenAtRelease;

extern "C" void __fastcall
FrontEndCountedActionRelease_0059aa86(
    CCountedPointerNUISystemCAction_0059aa86* holder,
    void*)
{
    ++g_ReleaseCalls;
    g_ActionSeenAtRelease = holder->action;
    g_InfoSeenAtRelease = holder->pointer_info;
    holder->action = 0;
    holder->pointer_info = 0;
}

static void ResetProbe()
{
    g_ReleaseCalls = 0;
    g_ActionSeenAtRelease = 0;
    g_InfoSeenAtRelease = 0;
}

int main()
{
    int failures = 0;
    int old_action = 11;
    int new_action = 22;
    int alternate_action = 33;

    FrontEndActionPointerInfo_0059aa86 old_info;
    old_info.reference_count = 3;
    old_info.destroy_callback = 0;
    old_info.action = &old_action;

    FrontEndActionPointerInfo_0059aa86 new_info;
    new_info.reference_count = 7;
    new_info.destroy_callback = 0;
    new_info.action = &new_action;

    ResetProbe();
    CCountedPointerNUISystemCAction_0059aa86 same_owner;
    same_owner.action = &old_action;
    same_owner.pointer_info = &old_info;
    same_owner.ShareData(&alternate_action, &old_info);
    if (g_ReleaseCalls != 0 ||
        same_owner.action != &old_action ||
        same_owner.pointer_info != &old_info ||
        old_info.reference_count != 3)
    {
        std::printf("identity short-circuit failed\n");
        ++failures;
    }

    ResetProbe();
    CCountedPointerNUISystemCAction_0059aa86 replacement;
    replacement.action = &old_action;
    replacement.pointer_info = &old_info;
    replacement.ShareData(&new_action, &new_info);
    if (g_ReleaseCalls != 1 ||
        g_ActionSeenAtRelease != &old_action ||
        g_InfoSeenAtRelease != &old_info ||
        replacement.action != &new_action ||
        replacement.pointer_info != &new_info ||
        new_info.reference_count != 8)
    {
        std::printf("release-before-rebind/acquire failed\n");
        ++failures;
    }

    ResetProbe();
    CCountedPointerNUISystemCAction_0059aa86 null_owner;
    null_owner.action = &new_action;
    null_owner.pointer_info = &new_info;
    null_owner.ShareData(&alternate_action, 0);
    if (g_ReleaseCalls != 1 ||
        g_ActionSeenAtRelease != &new_action ||
        g_InfoSeenAtRelease != &new_info ||
        null_owner.action != &alternate_action ||
        null_owner.pointer_info != 0 ||
        new_info.reference_count != 8)
    {
        std::printf("null pointer-info rebind failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059aa86_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059aa86_TEST PASS\n");
    return 0;
}
