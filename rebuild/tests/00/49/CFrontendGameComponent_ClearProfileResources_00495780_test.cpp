#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ClearProfileResources_00495780.cpp"

class CFrontendProfileContext_00495780
{
public:
    void Destroy();
};

struct CFrontendProfileControl_00495780
{
    long references;
    void (CFrontendProfileContext_00495780::*destroy)();
    CFrontendProfileContext_00495780* context;
};

static unsigned long g_DestroyCalls;
static unsigned long g_DeleteCalls;
static unsigned long g_ClearCalls;
static void* g_Deleted;
static void* g_ClearThis[2];
static void* g_ClearBegin[2];
static void* g_ClearEnd[2];

void CFrontendProfileContext_00495780::Destroy()
{
    ++g_DestroyCalls;
}

void __cdecl FrontendProfileCountedDelete_00495780(void* control)
{
    ++g_DeleteCalls;
    g_Deleted = control;
}

void CFrontendProfileRange_00495780::Clear(void* begin, void* end)
{
    if (g_ClearCalls < 2)
    {
        g_ClearThis[g_ClearCalls] = this;
        g_ClearBegin[g_ClearCalls] = begin;
        g_ClearEnd[g_ClearCalls] = end;
    }
    ++g_ClearCalls;
}

int main()
{
    unsigned char storage[0x1e38];
    CFrontendGameComponent_00495780* component =
        reinterpret_cast<CFrontendGameComponent_00495780*>(storage);
    unsigned long firstRange[3] = { 1, 2, 3 };
    unsigned long secondRange[2] = { 4, 5 };
    CFrontendProfileContext_00495780 firstContext;
    CFrontendProfileContext_00495780 secondContext;
    CFrontendProfileControl_00495780 firstControl = {
        1, &CFrontendProfileContext_00495780::Destroy, &firstContext
    };
    CFrontendProfileControl_00495780 secondControl = {
        2, &CFrontendProfileContext_00495780::Destroy, &secondContext
    };

    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<void**>(storage + 0x1e04) =
        reinterpret_cast<void*>(0x11111111);
    *reinterpret_cast<CFrontendProfileControl_00495780**>(
        storage + 0x1e08) = &firstControl;
    *reinterpret_cast<void**>(storage + 0x1dfc) =
        reinterpret_cast<void*>(0x22222222);
    *reinterpret_cast<CFrontendProfileControl_00495780**>(
        storage + 0x1e00) = &secondControl;
    *reinterpret_cast<unsigned long**>(storage + 0x1e1c) = firstRange;
    *reinterpret_cast<unsigned long**>(storage + 0x1e20) = firstRange + 3;
    *reinterpret_cast<unsigned long**>(storage + 0x1e30) = secondRange;
    *reinterpret_cast<unsigned long**>(storage + 0x1e34) = secondRange + 2;

    component->ClearProfileResources();

    if (g_DestroyCalls != 1 || g_DeleteCalls != 1 ||
        g_Deleted != &firstControl || firstControl.references != 0 ||
        secondControl.references != 1 ||
        *reinterpret_cast<void**>(storage + 0x1e04) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1e08) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1dfc) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1e00) != 0 ||
        g_ClearCalls != 2 ||
        g_ClearThis[0] != storage + 0x1e1c ||
        g_ClearBegin[0] != firstRange ||
        g_ClearEnd[0] != firstRange + 3 ||
        g_ClearThis[1] != storage + 0x1e30 ||
        g_ClearBegin[1] != secondRange ||
        g_ClearEnd[1] != secondRange + 2)
    {
        std::printf(
            "CFrontendGameComponent_ClearProfileResources_00495780_test FAIL\n");
        return 1;
    }

    std::printf(
        "CFrontendGameComponent_ClearProfileResources_00495780_test PASS\n");
    return 0;
}
