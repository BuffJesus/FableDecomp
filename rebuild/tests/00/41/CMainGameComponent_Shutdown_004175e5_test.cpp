// Behaviour fixture for CMainGameComponent::Shutdown @ 0x004175E5.
// Every callee is a recording stand-in; the component is a raw retail-sized
// buffer (offsets from the retail body). Checks receivers, arguments, call
// order, field writes, untouched neighbours and each branch (sound manager
// present/absent, world present/absent, client valid/invalid, owned
// subsystems present/absent).
#include "fable_game.h"
#include "fable_console.h"
#include "test_support/console_member_stubs.h"
#include "fable_definition_manager.h"
#include "fable_display_engine.h"
#include "fable_particle_engine.h"
#include "fable_player_gui.h"
#include "fable_player_manager.h"
#include "fable_system_manager.h"
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

CASoundManager* g_FableSoundManager_013B8394;
CDisplayManager* g_FableDisplayManager_013B8390;
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;
CMainGameComponent* CMainGameComponent::PMainGameComponent;
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;

// Reaches the protected members under test (never constructed).
struct MainGameProbe : CMainGameComponent
{
    static void CallShutdown(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->Shutdown(); }
    static CMainGameComponent*& PMain() { return PMainGameComponent; }
};

static int seq;
static bool ok;
static unsigned char* comp;
static unsigned char compStorage[0x161E8 + 16];
static void* guiPClass;

// Test-only window onto the private PClass (explicit specialization of the
// public accessor; the source object never calls Get).
template<>
CPlayerGui& FABLE_FASTCALL CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()
{
    return *reinterpret_cast<CPlayerGui*>(&PClass);
}

static unsigned long Rd(unsigned off) { unsigned long v; memcpy(&v, comp + off, 4); return v; }
static void Wr(unsigned off, unsigned long v) { memcpy(comp + off, &v, 4); }

// ---- recorded events ----
enum
{
    E_SLOT16, E_SLOT18, E_UPDATE, E_WORLD_UNINIT, E_CLEAR, E_GUI_RELEASE, E_REMOVE_CB,
    E_SMOOTHER, E_ISVALID, E_CLIENT_UNINIT, E_UNINIT_LOAD, E_UNINIT_SAVE, E_PARTICLE,
    E_CONSOLE_INIT, E_TEXT_RELEASE, E_DEL_PM, E_DEL_PI, E_DEL_WORLD, E_DEL_DE,
    E_LUA_RELEASE, E_DEBUGFONT_RELEASE, E_CUTFONT_RELEASE, E_GFX_RELEASE, E_DEFSET, E_COUNT
};
static int at[E_COUNT];
static void Hit(int e) { if (at[e]) ok = false; at[e] = ++seq; }

static long slot18Arg;
static char consoleChar;
static int consoleKey;
static const void* consoleFont;
static bool clientValid;
static unsigned long delFlags[4];

// ---- sound manager: hand-built vtable ----
static unsigned long soundObject[0x40];
static void* soundVtable[60];
static void FABLE_FASTCALL Slot16(void* self, void*)
{
    Hit(E_SLOT16);
    ok = ok && self == soundObject && comp[0x161E0] == 0;
}
static void FABLE_FASTCALL Slot18(void* self, void*, long arg)
{
    Hit(E_SLOT18);
    slot18Arg = arg;
    ok = ok && self == soundObject;
}
static void FABLE_FASTCALL SlotBad(void*, void*) { ok = false; }
void CASoundManager::UnknownUpdateEntries_00991750()
{
    Hit(E_UPDATE);
    ok = ok && static_cast<void*>(this) == soundObject;
}
void CASoundManager::UnknownClearEntries_009918F0()
{
    Hit(E_CLEAR);
    ok = ok && static_cast<void*>(this) == soundObject;
}

// ---- owned polymorphic objects: hand-built vtables ----
static unsigned long worldObject[4], pmObject[4], piObject[4], deObject[4];
static void* worldVtable[16];
static void* pmVtable[4];
static void* piVtable[4];
static void* deVtable[4];
static void* FABLE_FASTCALL DelWorld(void* self, void*, unsigned flags)
{
    Hit(E_DEL_WORLD); delFlags[2] = flags;
    ok = ok && self == worldObject && Rd(0x24) == 0;
    return self;
}
static void* FABLE_FASTCALL DelPM(void* self, void*, unsigned flags)
{
    Hit(E_DEL_PM); delFlags[0] = flags;
    ok = ok && self == pmObject && Rd(0x1C) == 0 && Rd(0x14) == 0;
    return self;
}
static void* FABLE_FASTCALL DelPI(void* self, void*, unsigned flags)
{
    Hit(E_DEL_PI); delFlags[1] = flags;
    ok = ok && self == piObject && Rd(0x20) == 0;
    return self;
}
static void* FABLE_FASTCALL DelDE(void* self, void*, unsigned flags)
{
    Hit(E_DEL_DE); delFlags[3] = flags;
    ok = ok && self == deObject && Rd(0x28) == 0;
    return self;
}
static void FABLE_FASTCALL WorldUninit(void* self, void*)
{
    Hit(E_WORLD_UNINIT);
    ok = ok && self == worldObject;
}

// ---- counted-pointer releases (do NOT clear Data: the source must) ----
template<> void CCountedPointer<CPlayerGui>::ReleaseData()
{
    Hit(E_GUI_RELEASE);
    ok = ok && static_cast<void*>(this) == guiPClass && Rd(0x10) == 0;
}
template<> void CCountedPointer<NGameText::CDataBank>::ReleaseData()
{
    Hit(E_TEXT_RELEASE);
    ok = ok && static_cast<void*>(this) == comp + 0x14;
}
template<> void CCountedPointer<CLUA>::ReleaseData()
{
    Hit(E_LUA_RELEASE);
    ok = ok && static_cast<void*>(this) == comp + 0x2C && Rd(0x28) == 0;
}
template<> void CCountedPointer<CFontBank>::ReleaseData()
{
    if (static_cast<void*>(this) == comp + 0x16144)
    {
        Hit(E_DEBUGFONT_RELEASE);
        ok = ok && Rd(0x2C) == 0;
    }
    else if (static_cast<void*>(this) == comp + 0x1614C)
    {
        Hit(E_CUTFONT_RELEASE);
        ok = ok && Rd(0x16144) == 0;
    }
    else
        ok = false;
}
template<> void CCountedPointer<CGraphicDataBank>::ReleaseData()
{
    Hit(E_GFX_RELEASE);
    ok = ok && static_cast<void*>(this) == comp + 0x161C8 && Rd(0x1614C) == 0;
}

// ---- remaining callees ----
static unsigned long displayObject[0x2C8 / 4];
void CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback)
{
    Hit(E_REMOVE_CB);
    ok = ok && static_cast<void*>(this) == displayObject &&
        static_cast<void*>(callback) == comp + 4 &&
        MainGameProbe::PMain() == 0 &&
        reinterpret_cast<unsigned long*>(guiPClass)[0] == 0;
}
void CFrameRateSmoother::Uninitialise()
{
    Hit(E_SMOOTHER);
    ok = ok && static_cast<void*>(this) == comp + 0x16178;
}
bool CInitBaseClass::IsValid() const
{
    Hit(E_ISVALID);
    ok = ok && static_cast<const void*>(this) == comp + 0x13AB8;
    return clientValid;
}
void CNetworkClient::Uninitialise()
{
    Hit(E_CLIENT_UNINIT);
    ok = ok && static_cast<void*>(this) == comp + 0x13AB8;
}
void CMainGameComponent::UninitEventPackageLoading()
{
    Hit(E_UNINIT_LOAD);
    ok = ok && static_cast<void*>(this) == comp;
}
void CMainGameComponent::UninitEventPackageSaving()
{
    Hit(E_UNINIT_SAVE);
    ok = ok && static_cast<void*>(this) == comp;
}
void FABLE_FASTCALL NParticleEngine::Uninitialise() { Hit(E_PARTICLE); }

static unsigned char consoleStorage[0x9C];
CConsole::CConsole() { ok = false; }
template<> void CCountedPointer<CConsole>::Reset(CConsole*) { ok = false; }
void CConsole::Initialise(char activateKeyChar, EInputKey activateKey, const CFontBank* font)
{
    Hit(E_CONSOLE_INIT);
    consoleChar = activateKeyChar;
    consoleKey = activateKey;
    consoleFont = font;
    ok = ok && static_cast<void*>(this) == consoleStorage;
}
void FABLE_FASTCALL CGameDefinitionManager::Set(CGameDefinitionManager* manager)
{
    Hit(E_DEFSET);
    ok = ok && manager == 0 && Rd(0x161C8) == 0;
}

static bool Run(bool haveSound, bool haveWorld, bool valid, bool haveOwned)
{
    comp = compStorage;
    memset(compStorage, 0xA5, sizeof(compStorage));
    comp[0x161E0] = 1;                                   // Initialised
    Wr(0x10, 0x5A5A0010);                                // PSampleBank
    Wr(0x14, 0x11110014); Wr(0x18, 0x11110018);          // PTextBank {Data, Info}
    Wr(0x1C, haveOwned ? (unsigned long)pmObject : 0);
    Wr(0x20, haveOwned ? (unsigned long)piObject : 0);
    Wr(0x24, haveWorld ? (unsigned long)worldObject : 0);
    Wr(0x28, haveOwned ? (unsigned long)deObject : 0);
    Wr(0x2C, 0x2222002C); Wr(0x30, 0x22220030);          // PLUA
    Wr(0x16144, 0x33316144); Wr(0x16148, 0x33316148);    // PDebugFont
    Wr(0x1614C, 0x4441614C); Wr(0x16150, 0x44416150);    // PCutSceneMainFont
    Wr(0x161C8, 0x555161C8); Wr(0x161CC, 0x555161CC);    // PMainGraphicBank
    unsigned char before[sizeof(compStorage)];
    memcpy(before, compStorage, sizeof(compStorage));

    for (int i = 0; i < 60; ++i) soundVtable[i] = (void*)&SlotBad;
    soundVtable[0x40 / 4] = (void*)&Slot16;
    soundVtable[0x48 / 4] = (void*)&Slot18;
    soundObject[0] = (unsigned long)soundVtable;
    g_FableSoundManager_013B8394 = haveSound ? reinterpret_cast<CASoundManager*>(soundObject) : 0;
    g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayObject);

    for (int i = 0; i < 16; ++i) worldVtable[i] = (void*)&SlotBad;
    worldVtable[0] = (void*)&DelWorld;
    worldVtable[10] = (void*)&WorldUninit;
    worldObject[0] = (unsigned long)worldVtable;
    pmVtable[0] = (void*)&DelPM; pmObject[0] = (unsigned long)pmVtable;
    piVtable[0] = (void*)&DelPI; piVtable[1] = (void*)&SlotBad; piObject[0] = (unsigned long)piVtable;
    deVtable[0] = (void*)&DelDE; deVtable[1] = (void*)&SlotBad; deVtable[2] = (void*)&SlotBad;
    deObject[0] = (unsigned long)deVtable;

    guiPClass = &CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get();
    reinterpret_cast<unsigned long*>(guiPClass)[0] = 0x6666AAAA;
    reinterpret_cast<unsigned long*>(guiPClass)[1] = 0x6666BBBB;
    MainGameProbe::PMain() = reinterpret_cast<CMainGameComponent*>(comp);
    g_FableConsolePClass_013CAA40.Data = reinterpret_cast<CConsole*>(consoleStorage);

    memset(at, 0, sizeof(at));
    memset(delFlags, 0, sizeof(delFlags));
    seq = 0;
    slot18Arg = -1;
    consoleChar = 0; consoleKey = 0; consoleFont = 0;
    clientValid = valid;
    ok = true;

    MainGameProbe::CallShutdown(reinterpret_cast<CMainGameComponent*>(comp));

    // Call order.
    int expected[E_COUNT];
    int n = 0;
    for (int e = 0; e < E_COUNT; ++e) expected[e] = 0;
    if (haveSound) { expected[E_SLOT16] = ++n; expected[E_SLOT18] = ++n; expected[E_UPDATE] = ++n; }
    if (haveWorld) expected[E_WORLD_UNINIT] = ++n;
    if (haveSound) expected[E_CLEAR] = ++n;
    expected[E_GUI_RELEASE] = ++n;
    expected[E_REMOVE_CB] = ++n;
    expected[E_SMOOTHER] = ++n;
    expected[E_ISVALID] = ++n;
    if (valid) expected[E_CLIENT_UNINIT] = ++n;
    expected[E_UNINIT_LOAD] = ++n;
    expected[E_UNINIT_SAVE] = ++n;
    expected[E_PARTICLE] = ++n;
    expected[E_CONSOLE_INIT] = ++n;
    expected[E_TEXT_RELEASE] = ++n;
    if (haveOwned) expected[E_DEL_PM] = ++n;
    if (haveOwned) expected[E_DEL_PI] = ++n;
    if (haveWorld) expected[E_DEL_WORLD] = ++n;
    if (haveOwned) expected[E_DEL_DE] = ++n;
    expected[E_LUA_RELEASE] = ++n;
    expected[E_DEBUGFONT_RELEASE] = ++n;
    expected[E_CUTFONT_RELEASE] = ++n;
    expected[E_GFX_RELEASE] = ++n;
    expected[E_DEFSET] = ++n;
    for (int e = 0; e < E_COUNT; ++e)
        if (at[e] != expected[e])
        {
            printf("event %d at %d expected %d\n", e, at[e], expected[e]);
            ok = false;
        }

    // Arguments.
    if (haveSound && slot18Arg != 0) ok = false;
    if (consoleChar != '`' || consoleKey != 0x29 || consoleFont != (const void*)0x33316144) ok = false;
    for (int i = 0; i < 4; ++i)
        if (at[E_DEL_PM + i] && delFlags[i] != 1) ok = false;

    // Field writes: exactly these changed; Info words and everything else untouched.
    unsigned char want[sizeof(compStorage)];
    memcpy(want, before, sizeof(before));
    want[0x161E0] = 0;
    memset(want + 0x10, 0, 4);
    memset(want + 0x14, 0, 4);
    memset(want + 0x1C, 0, 16);
    memset(want + 0x2C, 0, 4);
    memset(want + 0x16144, 0, 4);
    memset(want + 0x1614C, 0, 4);
    memset(want + 0x161C8, 0, 4);
    if (memcmp(want, compStorage, sizeof(compStorage)) != 0) { printf("field mismatch\n"); ok = false; }
    if (reinterpret_cast<unsigned long*>(guiPClass)[0] != 0 ||
        reinterpret_cast<unsigned long*>(guiPClass)[1] != 0x6666BBBB) ok = false;
    // The fake Info word must not reach the real destructor at exit.
    reinterpret_cast<unsigned long*>(guiPClass)[1] = 0;
    g_FableConsolePClass_013CAA40.Data = 0;
    if (MainGameProbe::PMain() != 0) ok = false;
    return ok;
}

int main()
{
    bool pass = true;
    pass = Run(true, true, true, true) && pass;
    pass = Run(false, true, true, true) && pass;
    pass = Run(true, false, false, true) && pass;
    pass = Run(false, false, true, false) && pass;
    pass = Run(true, true, false, false) && pass;
    if (pass)
        printf("FABLETLC_MAINGAME_SHUTDOWN PASS\n");
    else
        printf("FABLETLC_MAINGAME_SHUTDOWN FAIL\n");
    return pass ? 0 : 1;
}