#include <stdio.h>
#include <string.h>

#include "fable_nui_manager.h"

// Behaviour fixture for NUISystem::CManager::ClearErrorMessage @ 0x0041CE6A.
// Retail offsets: m_bHasErrorMessage +0x9C, m_pErrorMessage +0xA0. The error
// component is a fake object whose vtable has the scalar deleting dtor in
// slot 0 and ChangeState(unsigned long) in slot 48 (+0xC0); __fastcall
// stand-ins take `this` in ecx and pop their stack argument like thiscall.

struct FakeComponent
{
    void** vtable;
};

static int g_seq, g_stateAt, g_deleteAt;
static void* g_stateThis;
static void* g_deleteThis;
static unsigned long g_stateArg;
static unsigned int g_deleteFlag;

static void* __fastcall FakeDelete(void* self, void*, unsigned int flags)
{
    g_deleteAt = ++g_seq; g_deleteThis = self; g_deleteFlag = flags;
    return self;
}

static void __fastcall FakeChangeState(void* self, void*, unsigned long state)
{
    g_stateAt = ++g_seq; g_stateThis = self; g_stateArg = state;
}

static void* g_vtable[64];
static FakeComponent g_component;
static unsigned char g_manager[0xD0];

int main()
{
    for (int i = 0; i < 64; ++i) g_vtable[i] = 0;
    g_vtable[0] = (void*)&FakeDelete;
    g_vtable[48] = (void*)&FakeChangeState;
    g_component.vtable = g_vtable;

    memset(g_manager, 0x77, sizeof(g_manager));
    g_manager[0x9C] = 1;
    FakeComponent* p = &g_component;
    memcpy(g_manager + 0xA0, &p, 4);
    g_seq = g_stateAt = g_deleteAt = 0;
    g_stateArg = 99; g_deleteFlag = 99;

    reinterpret_cast<NUISystem::CManager*>(g_manager)->ClearErrorMessage();

    bool ok = true;
    ok = ok && g_manager[0x9C] == 0;
    void* after; memcpy(&after, g_manager + 0xA0, 4);
    ok = ok && after == 0;
    ok = ok && g_stateAt == 1 && g_stateThis == &g_component && g_stateArg == 0;
    ok = ok && g_deleteAt == 2 && g_deleteThis == &g_component && g_deleteFlag == 1;
    for (int i = 0; i < 0xD0; ++i)
    {
        if (i == 0x9C || (i >= 0xA0 && i < 0xA4)) continue;
        if (g_manager[i] != 0x77) { ok = false; break; }
    }
    if (ok) printf("CManager_0041ce6a_TEST PASS\n");
    else printf("FAIL state@%d(%lu) delete@%d(%u)\n", g_stateAt, g_stateArg, g_deleteAt, g_deleteFlag);
    return ok ? 0 : 1;
}
