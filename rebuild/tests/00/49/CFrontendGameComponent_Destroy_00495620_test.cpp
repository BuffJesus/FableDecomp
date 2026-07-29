#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_Destroy_00495620.cpp"

unsigned long g_CFrontendDerivedVTable0_00495620;
unsigned long g_CFrontendDerivedVTable4_00495620;
unsigned long g_CFrontendBaseVTable0_00495620;
unsigned long g_CFrontendBaseVTable4_00495620;

class CFrontendCountedContext_00495620
{
public:
    void Destroy();
};

struct CFrontendCountedControl_00495620
{
    long references;
    void (CFrontendCountedContext_00495620::*destroy)();
    CFrontendCountedContext_00495620* context;
};

static unsigned long g_SubobjectCalls;
static unsigned long g_StringCalls;
static unsigned long g_VectorFreeCalls;
static unsigned long g_ContextDestroyCalls;
static unsigned long g_CountedDeleteCalls;
static unsigned long g_BaseCalls;
static void* g_SubobjectThis;
static void* g_StringThis[8];
static void* g_VectorFreed[3];
static void* g_CountedDeleted[2];
static void* g_BaseThis;

void CFrontendCleanupObject_00495620::DestroySubobject()
{
    ++g_SubobjectCalls;
    g_SubobjectThis = this;
}

void CFrontendCleanupObject_00495620::DestroyString()
{
    if (g_StringCalls < 8)
        g_StringThis[g_StringCalls] = this;
    ++g_StringCalls;
}

void CFrontendCleanupObject_00495620::DestroyBase()
{
    ++g_BaseCalls;
    g_BaseThis = this;
}

void CFrontendCountedContext_00495620::Destroy()
{
    ++g_ContextDestroyCalls;
}

void __cdecl FrontendVectorFree_00495620(void* allocation)
{
    if (g_VectorFreeCalls < 3)
        g_VectorFreed[g_VectorFreeCalls] = allocation;
    ++g_VectorFreeCalls;
}

void __cdecl FrontendCountedDelete_00495620(void* allocation)
{
    if (g_CountedDeleteCalls < 2)
        g_CountedDeleted[g_CountedDeleteCalls] = allocation;
    ++g_CountedDeleteCalls;
}

int main()
{
    unsigned char storage[0x1e5c];
    CFrontendGameComponent_00495620* component =
        reinterpret_cast<CFrontendGameComponent_00495620*>(storage);
    unsigned long first[2] = { 1, 2 };
    unsigned long second[2] = { 3, 4 };
    unsigned long third[2] = { 5, 6 };
    CFrontendCountedContext_00495620 firstContext;
    CFrontendCountedContext_00495620 secondContext;
    CFrontendCountedControl_00495620 firstControl = {
        1, &CFrontendCountedContext_00495620::Destroy, &firstContext
    };
    CFrontendCountedControl_00495620 secondControl = {
        1, &CFrontendCountedContext_00495620::Destroy, &secondContext
    };

    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<unsigned long**>(storage + 0x1e30) = first;
    *reinterpret_cast<unsigned long**>(storage + 0x1e34) = first + 2;
    *reinterpret_cast<unsigned long**>(storage + 0x1e1c) = second;
    *reinterpret_cast<unsigned long**>(storage + 0x1e20) = second + 2;
    *reinterpret_cast<unsigned long**>(storage + 0x1e0c) = third;
    *reinterpret_cast<unsigned long**>(storage + 0x1e10) = third + 2;
    *reinterpret_cast<void**>(storage + 0x1e04) =
        reinterpret_cast<void*>(0x11111111);
    *reinterpret_cast<CFrontendCountedControl_00495620**>(
        storage + 0x1e08) = &firstControl;
    *reinterpret_cast<void**>(storage + 0x1dfc) =
        reinterpret_cast<void*>(0x22222222);
    *reinterpret_cast<CFrontendCountedControl_00495620**>(
        storage + 0x1e00) = &secondControl;

    component->Destroy();

    if (g_SubobjectCalls != 1 || g_SubobjectThis != storage + 0x1e58 ||
        g_StringCalls != 8 || g_VectorFreeCalls != 3 ||
        g_VectorFreed[0] != first || g_VectorFreed[1] != second ||
        g_VectorFreed[2] != third || g_ContextDestroyCalls != 2 ||
        g_CountedDeleteCalls != 2 ||
        g_CountedDeleted[0] != &firstControl ||
        g_CountedDeleted[1] != &secondControl ||
        firstControl.references != 0 || secondControl.references != 0 ||
        *reinterpret_cast<void**>(storage + 0x1e04) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1e08) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1dfc) != 0 ||
        *reinterpret_cast<void**>(storage + 0x1e00) != 0 ||
        *reinterpret_cast<unsigned long*>(storage) !=
            reinterpret_cast<unsigned long>(&g_CFrontendBaseVTable0_00495620) ||
        *reinterpret_cast<unsigned long*>(storage + 4) !=
            reinterpret_cast<unsigned long>(&g_CFrontendBaseVTable4_00495620) ||
        g_BaseCalls != 1 || g_BaseThis != storage)
    {
        std::printf("CFrontendGameComponent_Destroy_00495620_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendGameComponent_Destroy_00495620_test PASS\n");
    return 0;
}
