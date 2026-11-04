// Behaviour fixture for CMainGameComponent::InitDefinitions @ 0x00416005.
// Checks, for allocator present/absent x compile true/false: the allocator
// total is sampled before and after (only when the allocator exists), the
// definition manager's slot-2 virtual (InitAndCompile) runs on the manager
// Get() returns, ClearSymbols runs on it only when compiling, the call order,
// the return value (always true) and that the component is untouched.
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static bool Call(CMainGameComponent* c, bool compile) { return static_cast<MainGameProbe*>(c)->InitDefinitions(compile); }
};

CMemoryAllocator* g_FableMemoryAllocator_013CAA90;

static unsigned char comp[0x161F0];
static unsigned char allocator[0x420];
static void* mgrVtbl[4];
static void* mgr[0xE0 / 4];
static int seq, totalAt[4], totalCalls, getAt[4], getCalls, compileAt, compileCalls, clearAt, clearCalls, badCalls;
static bool ok;

unsigned long CMemoryAllocator::GetTotalMemoryAllocated()
{
    ok = ok && static_cast<void*>(this) == allocator;
    if (totalCalls < 4) totalAt[totalCalls] = ++seq;
    ++totalCalls;
    return 1000;
}
CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    if (getCalls < 4) getAt[getCalls] = ++seq;
    ++getCalls;
    return *reinterpret_cast<CGameDefinitionManager*>(mgr);
}
void CDefinitionManager::ClearSymbols()
{
    ok = ok && static_cast<void*>(this) == mgr;
    clearAt = ++seq;
    ++clearCalls;
}
static void __fastcall FakeInitAndCompile(void* self, void*)
{
    ok = ok && self == mgr;
    compileAt = ++seq;
    ++compileCalls;
}
static void __fastcall FakeBad(void*, void*) { ++badCalls; }

static bool Run(bool haveAllocator, bool compile)
{
    for (int i = 0; i < 4; ++i) mgrVtbl[i] = (void*)FakeBad;
    mgrVtbl[2] = (void*)FakeInitAndCompile;
    memset(mgr, 0, sizeof(mgr));
    mgr[0] = mgrVtbl;
    g_FableMemoryAllocator_013CAA90 = haveAllocator ? reinterpret_cast<CMemoryAllocator*>(allocator) : 0;
    memset(comp, 0x77, sizeof(comp));
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    seq = totalCalls = getCalls = compileCalls = clearCalls = badCalls = 0;
    compileAt = clearAt = 0;
    memset(totalAt, 0, sizeof(totalAt));
    memset(getAt, 0, sizeof(getAt));
    ok = true;

    bool result = MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp), compile);

    int s = 0;
    if (haveAllocator) ok = ok && totalAt[0] == ++s;
    ok = ok && getAt[0] == ++s && compileAt == ++s;
    if (compile) ok = ok && getAt[1] == ++s && clearAt == ++s;
    if (haveAllocator) ok = ok && totalAt[1] == ++s;
    ok = ok && result && badCalls == 0 && compileCalls == 1
        && clearCalls == (compile ? 1 : 0) && getCalls == (compile ? 2 : 1)
        && totalCalls == (haveAllocator ? 2 : 0) && seq == s
        && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case alloc=%d compile=%d failed\n", haveAllocator, compile);
    return ok;
}

int main()
{
    bool pass = Run(true, true);
    pass = Run(true, false) && pass;
    pass = Run(false, true) && pass;
    pass = Run(false, false) && pass;
    printf(pass ? "PASS_00416005\n" : "FAIL_00416005\n");
    return pass ? 0 : 1;
}
