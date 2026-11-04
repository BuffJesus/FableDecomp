// Behaviour fixture for CMainGameComponent::InitPlayerManager @ 0x0041732A.
// PPlayerManager is +0x1C (a boost::scoped_ptr). Checks: 0x2C bytes are
// allocated; on success the definition manager is fetched after the
// allocation and the new manager is constructed in the allocation with
// (this component, that manager); the old manager is deleted through its
// virtual scalar-deleting destructor (flag 1) only after the new pointer is
// ready, and PPlayerManager ends up holding the new one. A failed allocation
// skips the fetch and the constructor and leaves PPlayerManager null (the old
// manager is still deleted). No other component byte changes.
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_support/base_class_stubs.h"

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->InitPlayerManager(); }
};

static unsigned char comp[0x161F0];
static unsigned char defMgr[0xE0];
static unsigned char newManager[0x2C];
static unsigned long oldManager[4];
static void* oldVtbl[4];
static int seq, newAt, getAt, ctorAt, delAt, calls;
static unsigned newSize;
static unsigned delFlags;
static bool failAlloc, armed, ok;

void* operator new(size_t size)
{
    if (!armed) return malloc(size);
    newAt = ++seq; ++calls;
    newSize = (unsigned)size;
    return failAlloc ? 0 : newManager;
}
void operator delete(void* p)
{
    if (armed) ok = false;
    free(p);
}

CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    getAt = ++seq; ++calls;
    return *reinterpret_cast<CGameDefinitionManager*>(defMgr);
}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
// The recording manager keeps both vectors empty; string destruction is unused.
CCharString::~CCharString() { abort(); }
CPlayerManager::CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& definitionManager)
{
    ok = ok && static_cast<void*>(this) == newManager && static_cast<void*>(&component) == comp
        && static_cast<const void*>(&definitionManager) == defMgr;
    ctorAt = ++seq; ++calls;
}
CPlayerManager::~CPlayerManager() { ok = false; }

static unsigned long Rd(unsigned off) { unsigned long v; memcpy(&v, comp + off, 4); return v; }

static void* __fastcall OldDelete(void* self, void*, unsigned flags)
{
    ok = ok && self == oldManager;
    delFlags = flags;
    delAt = ++seq; ++calls;
    // The new pointer is swapped in before the old manager is deleted.
    ok = ok && Rd(0x1C) == (failAlloc ? 0 : (unsigned long)newManager);
    return self;
}
static void __fastcall Bad(void*, void*) { ok = false; }

static bool Run(bool fail)
{
    for (int i = 0; i < 4; ++i) oldVtbl[i] = (void*)Bad;
    oldVtbl[0] = (void*)OldDelete;
    oldManager[0] = (unsigned long)oldVtbl;
    memset(comp, 0x44, sizeof(comp));
    void* old = oldManager; memcpy(comp + 0x1C, &old, 4);
    static unsigned char expect[sizeof(comp)];
    memcpy(expect, comp, sizeof(comp));
    void* nm = fail ? 0 : (void*)newManager; memcpy(expect + 0x1C, &nm, 4);
    failAlloc = fail;
    seq = newAt = getAt = ctorAt = delAt = calls = 0;
    newSize = 0; delFlags = 0;
    ok = true;
    armed = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));
    armed = false;
    int s = 0;
    ok = ok && newSize == 0x2C && newAt == ++s;
    if (!fail) ok = ok && getAt == ++s && ctorAt == ++s;
    else ok = ok && getAt == 0 && ctorAt == 0;
    ok = ok && delAt == ++s && delFlags == 1 && calls == s && memcmp(expect, comp, sizeof(comp)) == 0;
    if (!ok) printf("case fail=%d failed\n", fail);
    return ok;
}

int main()
{
    bool pass = Run(false);
    pass = Run(true) && pass;
    printf(pass ? "PASS_0041732a\n" : "FAIL_0041732a\n");
    return pass ? 0 : 1;
}
