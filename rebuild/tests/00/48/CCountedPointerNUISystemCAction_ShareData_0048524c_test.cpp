#include <cstdio>

// Self-contained behaviour test for
// CCountedPointer<NUISystem::CAction>::ShareData @ 0x0048524C.
//
// The target ShareData is defined in source.obj (compiled with /O2 /Oy) and is
// pulled in by the source.obj + test.obj fallback link.  We MUST re-declare the
// exact same class/struct layout and method signature here so the call site
// emits the identical MSVC-mangled name that source.obj exports.  We also DEFINE
// stub bodies for the two masked callees that source.obj references, so the
// combined link resolves with no unresolved / duplicate symbols.

struct FrontEndActionPointerInfo_0048524c
{
    long reference_count;
    void* destroy_callback;
    void* action;
};

class CCountedPointerNUISystemCAction_0048524c
{
public:
    void* action;
    FrontEndActionPointerInfo_0048524c* pointer_info;

    // Declared, NOT defined here: the body lives in source.obj.
    void ShareData(
        void* new_action,
        FrontEndActionPointerInfo_0048524c* new_pointer_info);
};

// ---- Masked callee stubs (defined here so the link resolves) ---------------

static int g_ReleaseCalls;
static void* g_ActionSeenAtRelease;
static FrontEndActionPointerInfo_0048524c* g_InfoSeenAtRelease;

extern "C" void __fastcall
FrontEndCountedActionRelease_0048524c(
    CCountedPointerNUISystemCAction_0048524c* holder,
    void*)
{
    ++g_ReleaseCalls;
    g_ActionSeenAtRelease = holder->action;
    g_InfoSeenAtRelease = holder->pointer_info;
    holder->action = 0;
    holder->pointer_info = 0;
}

// Unreachable adjacent thunk; stub only so the link resolves.
extern "C" void __stdcall
FrontEndActionThunkForward_0048524c(void*)
{
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

    FrontEndActionPointerInfo_0048524c old_info;
    old_info.reference_count = 3;
    old_info.destroy_callback = 0;
    old_info.action = &old_action;

    FrontEndActionPointerInfo_0048524c new_info;
    new_info.reference_count = 7;
    new_info.destroy_callback = 0;
    new_info.action = &new_action;

    // Case 1: identity short-circuit -- new pointer_info == current pointer_info
    // -> no release, no rebind, no acquire.
    ResetProbe();
    CCountedPointerNUISystemCAction_0048524c same_owner;
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

    // Case 2: release old ownership, install both new fields, then acquire one
    // reference on the new (non-null) pointer_info.
    ResetProbe();
    CCountedPointerNUISystemCAction_0048524c replacement;
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

    // Case 3: rebind to a NULL pointer_info -> release runs, fields install,
    // but no acquire (null skip).
    ResetProbe();
    CCountedPointerNUISystemCAction_0048524c null_owner;
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
            "FRONTEND_0048524c_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0048524c_TEST PASS\n");
    return 0;
}