// Behaviour fixture for CMainGameComponent::InitWorld @ 0x0041735A.
// Retail offsets: PPlayerManager +0x1C, PWorld +0x24. The CWorld constructor
// and GEventPackageFileName are bound with /alternatename to recording
// stand-ins (CWorld's full vtable is not needed that way). Checks, for each
// load/save flag combination: the definition
// manager is fetched before the 0x198-byte allocation; the new world is
// constructed in it from a CWorldInit naming this component, the player
// manager and the definition manager with ScavengeWorld/DisplayEngine null;
// the old world is deleted through its vptr (flag 1) after PWorld holds the
// new one; InitEventPackageLoading/Saving run (in that order) only when their
// flag is set, each with GEventPackageFileName's text; the "Init World Init"
// progress line is shown with fraction -1 and both flags false, and its
// string is destroyed; finally the new world's slot-9 virtual (Init) runs.
// No other component byte changes. (A failed allocation is not exercised:
// retail then calls Init through the null PWorld.)
#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_player_manager.h"
#include "fable_progress_display.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(linker, "/alternatename:??0CWorld@@QAE@ABVCWorldInit@@@Z=@FakeWorldCtor@12")
#pragma comment(linker, "/alternatename:?g_EventPackageFileName_013B8660@@3VCWideString@@A=_fakeFileName")

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->InitWorld(); }
};

bool g_LoadEventPackages_013B85F8;
bool g_SaveEventPackages_013B85F7;
extern "C" unsigned long fakeFileName[4] = { 0x46494C45, 0, 0, 0 };
static const wchar_t fileText[] = L"events.pkg";

static unsigned char comp[0x161F0];
static unsigned char defMgr[0xE0];
static unsigned char playerManager[0x2C];
static unsigned long newWorld[0x198 / 4];
static void* newVtbl[16];
static unsigned long oldWorld[4];
static void* oldVtbl[16];
static int seq, getAt, newAt, ctorAt, delAt, loadAt, saveAt, strAt, progressAt, strDtorAt, initAt, textAt, textCalls, calls;
static unsigned newSize, delFlags;
static bool failAlloc, armed, ok;
static CWorldInit seenInit;
static const void* strThis;

static unsigned long Rd(unsigned off) { unsigned long v; memcpy(&v, comp + off, 4); return v; }

void* operator new(size_t size)
{
    if (!armed) return malloc(size);
    newAt = ++seq; ++calls;
    newSize = (unsigned)size;
    return failAlloc ? 0 : newWorld;
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
extern "C" void* __fastcall FakeWorldCtor(void* self, void*, const CWorldInit* init)
{
    ok = ok && self == newWorld;
    seenInit = *init;
    ctorAt = ++seq; ++calls;
    return self;
}
static void* __fastcall OldDelete(void* self, void*, unsigned flags)
{
    ok = ok && self == oldWorld && Rd(0x24) == (failAlloc ? 0 : (unsigned long)newWorld);
    delFlags = flags;
    delAt = ++seq; ++calls;
    return self;
}
static void __fastcall NewInit(void* self, void*)
{
    ok = ok && self == newWorld;
    initAt = ++seq; ++calls;
}
static void __fastcall Bad(void*, void*) { ok = false; }

CWideString::operator const wchar_t*() const
{
    ok = ok && static_cast<const void*>(this) == fakeFileName;
    textAt = ++seq; ++textCalls;
    return fileText;
}
bool CMainGameComponent::InitEventPackageLoading(const wchar_t* fileName)
{
    ok = ok && static_cast<void*>(this) == comp && fileName == fileText && textAt == seq;
    loadAt = ++seq; ++calls;
    return true;
}
bool CMainGameComponent::InitEventPackageSaving(const wchar_t* fileName)
{
    ok = ok && static_cast<void*>(this) == comp && fileName == fileText && textAt == seq;
    saveAt = ++seq; ++calls;
    return true;
}
CCharString::CCharString(const char* text, long length)
{
    ok = ok && strcmp(text, "Init World Init") == 0 && length == -1;
    strThis = this;
    strAt = ++seq; ++calls;
}
CCharString::~CCharString()
{
    ok = ok && static_cast<const void*>(this) == strThis;
    strDtorAt = ++seq; ++calls;
}
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    ok = ok && static_cast<const void*>(&text) == strThis && fraction == -1.0f && !forceRender && !isDebug;
    progressAt = ++seq; ++calls;
}

static bool Run(bool load, bool save, bool fail)
{
    for (int i = 0; i < 16; ++i) { newVtbl[i] = (void*)Bad; oldVtbl[i] = (void*)Bad; }
    newVtbl[9] = (void*)NewInit;
    oldVtbl[0] = (void*)OldDelete;
    memset(newWorld, 0, sizeof(newWorld));
    newWorld[0] = (unsigned long)newVtbl;
    oldWorld[0] = (unsigned long)oldVtbl;
    memset(comp, 0x5C, sizeof(comp));
    void* pm = playerManager; memcpy(comp + 0x1C, &pm, 4);
    void* ow = oldWorld; memcpy(comp + 0x24, &ow, 4);
    static unsigned char expect[sizeof(comp)];
    memcpy(expect, comp, sizeof(comp));
    void* nw = fail ? 0 : (void*)newWorld; memcpy(expect + 0x24, &nw, 4);
    g_LoadEventPackages_013B85F8 = load;
    g_SaveEventPackages_013B85F7 = save;
    failAlloc = fail;
    seq = getAt = newAt = ctorAt = delAt = loadAt = saveAt = strAt = progressAt = strDtorAt = initAt = textAt = textCalls = calls = 0;
    newSize = delFlags = 0;
    memset(&seenInit, 0xEE, sizeof(seenInit));
    ok = true;
    armed = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));
    armed = false;

    int s = 0;
    ok = ok && getAt == ++s && newAt == ++s && newSize == 0x198;
    if (!fail)
        ok = ok && ctorAt == ++s && seenInit.Component == reinterpret_cast<CMainGameComponent*>(comp)
            && seenInit.ScavengeWorld == 0
            && seenInit.PlayerManager == reinterpret_cast<CPlayerManager*>(playerManager)
            && seenInit.DefinitionManager == reinterpret_cast<CGameDefinitionManager*>(defMgr)
            && seenInit.DisplayEngine == 0;
    ok = ok && delAt == ++s && delFlags == 1;
    if (load) { ++s; ok = ok && loadAt == ++s; } else ok = ok && loadAt == 0;
    if (save) { ++s; ok = ok && saveAt == ++s; } else ok = ok && saveAt == 0;
    ok = ok && textCalls == (load ? 1 : 0) + (save ? 1 : 0);
    ok = ok && strAt == ++s && progressAt == ++s && strDtorAt == ++s && initAt == ++s
        && seq == s && memcmp(expect, comp, sizeof(comp)) == 0;
    if (!ok) printf("case load=%d save=%d fail=%d failed (seq %d)\n", load, save, fail, seq);
    return ok;
}

int main()
{
    bool pass = Run(false, false, false);
    pass = Run(true, false, false) && pass;
    pass = Run(false, true, false) && pass;
    pass = Run(true, true, false) && pass;
    printf(pass ? "INITWORLD_OK\n" : "INITWORLD_FAIL\n");
    return pass ? 0 : 1;
}
