#include <stdio.h>
#include <string.h>

#include "fable_nui_manager.h"

// Behaviour fixture for NUISystem::CFrontEndManager::ChangeStateFirstTime
// @ 0x005952C3: stack<CComponent*>::top() is called on m_stackScreens
// (+0x20) and the returned screen gets ChangeState(5) (vtable slot 48).

struct FakeComponent { void** vtable; };

static int g_seq, g_topAt, g_stateAt;
static void* g_topThis;
static void* g_stateThis;
static unsigned long g_stateArg;
static FakeComponent g_screen;
static NUISystem::CComponent* g_top;

template<>
_STL::stack<NUISystem::CComponent*>::reference
_STL::stack<NUISystem::CComponent*>::top()
{
    g_topAt = ++g_seq; g_topThis = this;
    return g_top;
}

static void __fastcall FakeChangeState(void* self, void*, unsigned long state)
{
    g_stateAt = ++g_seq; g_stateThis = self; g_stateArg = state;
}

static void* g_vtable[64];
static unsigned char g_manager[0xE0];

int main()
{
    memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[48] = (void*)&FakeChangeState;
    g_screen.vtable = g_vtable;
    g_top = reinterpret_cast<NUISystem::CComponent*>(&g_screen);
    memset(g_manager, 0x21, sizeof(g_manager));

    reinterpret_cast<NUISystem::CFrontEndManager*>(g_manager)->ChangeStateFirstTime();

    bool ok = g_topAt == 1 && g_topThis == g_manager + 0x20;
    ok = ok && g_stateAt == 2 && g_stateThis == &g_screen && g_stateArg == 5;
    for (int i = 0; i < 0xE0; ++i)
        if (g_manager[i] != 0x21) { ok = false; break; }
    if (ok) printf("FSE2_005952c3_TEST PASS\n");
    else printf("FAIL top@%d state@%d arg=%lu\n", g_topAt, g_stateAt, g_stateArg);
    return ok ? 0 : 1;
}
