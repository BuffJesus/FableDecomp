// Behaviour fixture for CMainGameComponent::Init @ 0x004184BD.
// Every callee is a recording stand-in; the component is raw 0xA5-filled
// storage with a fake vtable whose slot 8 (PostInit) records the call. The
// log checks call order, receivers and arguments; the storage snapshots check
// that exactly the expected fields change.
#include "fable_game.h"
#include "fable_console.h"
#include "fable_definition_manager.h"
#include "test_support/resource_list_stubs.h"
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
#include "fable_filesystem.h"
#include "fable_game_init_statics.h"
#include "fable_gfuninitialise.h"
#include "fable_player_manager.h"
#include "fable_profile.h"
#include "fable_progress_display.h"
#include "fable_world.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "test_support/base_class_stubs.h"

static char logText[8192];
static bool ok = true;
static void Log(const char* format, ...)
{
    char line[512];
    va_list args;
    va_start(args, format);
    _vsnprintf(line, sizeof(line) - 1, format, args);
    va_end(args);
    line[sizeof(line) - 1] = 0;
    strcat(logText, line);
    strcat(logText, ";");
}
static void Expect(bool condition, const char* what)
{
    if (!condition) { printf("FAIL: %s\n", what); ok = false; }
}

// ---- the component ------------------------------------------------------
static unsigned char storage[sizeof(CMainGameComponent)];
static unsigned char before[sizeof(CMainGameComponent)];
static CMainGameComponent* const obj = reinterpret_cast<CMainGameComponent*>(storage);
static unsigned char worldStorage[sizeof(CWorld)];
static unsigned char playerManagerStorage[0x2C];
static const CPlayerManager* const fakePlayerManager =
    reinterpret_cast<const CPlayerManager*>(playerManagerStorage);
static const CFontBank* const fakeFont = reinterpret_cast<const CFontBank*>(0x0F0F1234);

static void FABLE_FASTCALL PostInitSlot(void* self, void*) { Expect(self == obj, "PostInit receiver"); Log("PostInit"); }
static void FABLE_FASTCALL TrapSlot(void*, void*) { Log("TRAP"); ok = false; }
static void* fakeVtable[10];

CMainGameComponent* CMainGameComponent::PMainGameComponent;
static void (FABLE_FASTCALL *expectedReinit)(void*);

static CMainGameComponent** instanceSlot;
void CMainGameComponent::InitialiseText()
{
    Expect(this == obj && PMainGameComponent == obj, "InitialiseText receiver, instance published first");
    instanceSlot = &PMainGameComponent;
    Log("InitialiseText");
}
void CMainGameComponent::InitialiseFonts()    { Expect(this == obj, "InitialiseFonts receiver"); Log("InitialiseFonts"); }
bool CMainGameComponent::InitGraphics()       { Expect(this == obj, "InitGraphics receiver"); Log("InitGraphics"); return true; }
bool CMainGameComponent::InitDefinitions(bool compile) { Expect(this == obj, "InitDefinitions receiver"); Log("InitDefinitions(%d)", compile); return true; }
void CMainGameComponent::InitPlayerManager()
{
    Expect(this == obj, "InitPlayerManager receiver");
    Log("InitPlayerManager");
    *reinterpret_cast<const CPlayerManager**>(storage + 0x1C) = fakePlayerManager;
}
void CMainGameComponent::InitWorld()
{
    Expect(this == obj, "InitWorld receiver");
    Log("InitWorld(flag140=%02X)", worldStorage[0x140]);
    *reinterpret_cast<CWorld**>(storage + 0x24) = reinterpret_cast<CWorld*>(worldStorage);
}
void CMainGameComponent::InitDisplayEngine()  { Expect(this == obj, "InitDisplayEngine receiver"); Log("InitDisplayEngine"); }
void CMainGameComponent::CreatePlayers()      { Expect(this == obj, "CreatePlayers receiver"); Log("CreatePlayers"); }
void CMainGameComponent::InitSound()          { Expect(this == obj, "InitSound receiver"); Log("InitSound"); expectedReinit = &AppReinitFunc; }
void CMainGameComponent::InitParticleEngine() { Expect(this == obj, "InitParticleEngine receiver"); Log("InitParticleEngine"); }
unsigned long CMainGameComponent::PeekWorldChecksum() const { Expect(this == obj, "PeekWorldChecksum receiver"); Log("PeekWorldChecksum"); return 0x333; }
void FABLE_FASTCALL CMainGameComponent::AppReinitFunc(void*) { Log("TRAP AppReinitFunc"); ok = false; }

// ---- progress / profile / strings ----------------------------------------
static int liveStrings;
CCharString::CCharString(const char* text, long length)
{
    Expect(length == -1, "CCharString length");
    Expect(liveStrings == 0, "one string at a time");
    ++liveStrings;
    *reinterpret_cast<const char**>(this) = text;
}
CCharString::~CCharString() { --liveStrings; *reinterpret_cast<const char**>(this) = 0; }
static const char* Text(const CCharString& s) { return *reinterpret_cast<const char* const*>(&s); }

CWideString::CWideString(const wchar_t* text)
{
    Expect(liveStrings == 0, "one wide string at a time");
    ++liveStrings;
    *reinterpret_cast<const wchar_t**>(this) = text;
}
CWideString::~CWideString() { --liveStrings; *reinterpret_cast<const wchar_t**>(this) = 0; }
static const char* Narrow(const CWideString& s)
{
    static char buffer[64];
    const wchar_t* w = *reinterpret_cast<const wchar_t* const*>(&s);
    int i = 0;
    for (; w && w[i] && i < 63; ++i) buffer[i] = (char)w[i];
    buffer[i] = 0;
    return buffer;
}

namespace NProgressDisplay
{
    void FABLE_FASTCALL StartProgress(bool a, bool b, bool c, bool d, bool e, bool f)
    {
        Log("StartProgress(%d%d%d%d%d%d)", a, b, c, d, e, f);
    }
    void FABLE_FASTCALL SetToDisplayText(bool displayText) { Log("SetToDisplayText(%d)", displayText); }
    void FABLE_FASTCALL DisplayProgress(const CCharString& text, float fraction, bool forceRender, bool isDebug)
    {
        Log("Progress(%s,%g,%d,%d)", Text(text), fraction, forceRender, isDebug);
    }
}
namespace NProfileTimer
{
    void FABLE_FASTCALL StartProfile(const CCharString& label, long id) { Log("Profile(%s,%ld)", Text(label), id); }
}

// ---- console -------------------------------------------------------------
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;
static unsigned char consoleStorage[sizeof(CConsole)];
CConsole::CConsole() { Log("TRAP new CConsole"); ok = false; }
template<> void CCountedPointer<CConsole>::Reset(CConsole*) { Log("TRAP Reset"); ok = false; }
void CConsole::Initialise(char key, EInputKey inputKey, const CFontBank* font)
{
    Expect(this == reinterpret_cast<CConsole*>(consoleStorage), "console receiver");
    Log("Console.Initialise(%c,%d,%s)", key, (int)inputKey, font == fakeFont ? "debugfont" : "?");
}
void CConsole::RunScript(const CWideString& filename)
{
    Expect(this == reinterpret_cast<CConsole*>(consoleStorage), "console receiver");
    Log("RunScript(%s)", Narrow(filename));
}

// ---- globals ---------------------------------------------------------------
static unsigned char displayManagerStorage[0x400];
CDisplayManager* g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(displayManagerStorage);
unsigned long g_WorldFrame_013B7D70;
unsigned long g_RenderFrame_013B7D6C;
bool g_DeleteCheckPointSaveOnInitWorld_013B7C90;
fable_u8 g_FableCompileFrontendDefinitions_013B8648;
static unsigned char systemManagerStorage[sizeof(CSystemManager)];
CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Log("GFGetSystemManager");
    return reinterpret_cast<CSystemManager*>(systemManagerStorage);
}
double FABLE_FASTCALL GFGetTime() { Log("GFGetTime"); return 12.5; }

// ---- allocation / definitions ---------------------------------------------
static bool trackNew;
static void* lastNew;
void* operator new(size_t size)
{
    void* p = malloc(size ? size : 1);
    if (trackNew) { Log("new(%u)", (unsigned)size); lastNew = p; }
    return p;
}
void operator delete(void* p) { free(p); }

static bool definitionsAllocated;
bool FABLE_FASTCALL CGameDefinitionManager::IsAllocated() { Log("DefMgr.IsAllocated"); return definitionsAllocated; }
// CDefinitionManager's retail ctor (0x009B0470) builds real members; the
// stand-in is a member-free fastcall body under its mangled name (ecx = this,
// stack = name, callback), so nothing is constructed and the name is never read.
extern "C" void* __fastcall DefinitionManagerCtorStandIn(void* self, int, const void*, void*) { return self; }
#pragma comment(linker, "/alternatename:??0CDefinitionManager@@QAE@ABVCWideString@@P6IXABVCCharString@@@Z@Z=@DefinitionManagerCtorStandIn@16")
static unsigned long defMgrNameRaw;
CGameDefinitionManager::CGameDefinitionManager()
    : CDefinitionManager(*reinterpret_cast<const CWideString*>(&defMgrNameRaw), 0)
{ Expect(this == lastNew, "DefMgr ctor receiver"); Log("DefMgr.ctor"); }
// CDefinitionManager is polymorphic (retail vtable 0x01232C24); the stand-in
// constructor above emits the vtable, so its virtual members need bodies here.
CDefinitionManager::~CDefinitionManager() {}
void CGameDefinitionManager::InitAndCompile() { Expect(false, "InitAndCompile"); }
void FABLE_FASTCALL CGameDefinitionManager::Set(CGameDefinitionManager* manager)
{
    Expect(manager == lastNew, "DefMgr.Set argument");
    Log("DefMgr.Set");
}

void FABLE_FASTCALL CTCBase::RegisterTCs() { Log("RegisterTCs"); }
void FABLE_FASTCALL CSubtitledMessage::Initialise() { Log("SubtitledMessage.Initialise"); }
void FABLE_FASTCALL CConversationAttitude::Init() { Log("ConversationAttitude.Init"); }
void FABLE_FASTCALL CWorld::DeleteCheckPointSave() { Log("DeleteCheckPointSave(flag=%d)", g_DeleteCheckPointSaveOnInitWorld_013B7C90); }

// ---- player interface --------------------------------------------------------
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CInputProcessManager::~CInputProcessManager() {}
static void* constructedInterface;
CGamePlayerInterface::CGamePlayerInterface(const CMainGameComponent& component, const CPlayerManager& playerManager)
{
    Expect(this == lastNew, "interface ctor receiver");
    Expect(&component == obj, "interface component argument");
    Expect(&playerManager == fakePlayerManager, "interface player-manager argument");
    constructedInterface = this;
    Log("GPI.ctor");
}
CGamePlayerInterface::~CGamePlayerInterface() { Log("TRAP ~GPI"); ok = false; }
bool CGamePlayerInterface::GetGameEvent(CGameEvent&) const { return false; }

// ---- frame-rate smoother / network client / file system ---------------------
bool CFrameRateSmoother::Init(long maxNoTimes, long noFrames)
{
    Expect(reinterpret_cast<unsigned char*>(this) == storage + 0x16178, "smoother receiver");
    Log("Smoother.Init(%ld,%ld)", maxNoTimes, noFrames);
    return true;
}
void CNetworkClient::Update(long frame, unsigned long checksum, unsigned long seed)
{
    Expect(reinterpret_cast<unsigned char*>(this) == storage + 0x13AB8, "client receiver");
    Log("Client.Update(%lX,%lX,%lX)", frame, checksum, seed);
}
static bool defaultConfigExists;
bool FABLE_FASTCALL CAFile::PathExists(const CWideString& path)
{
    Log("PathExists(%s)", Narrow(path));
    return defaultConfigExists;
}

// ---- driver ------------------------------------------------------------------
static const char* const kPrefix =
    "StartProgress(100000);SetToDisplayText(0);InitialiseText;SetToDisplayText(1);"
    "Console.Initialise(`,41,debugfont);DefMgr.IsAllocated;";
static const char* const kNewDefs = "new(224);DefMgr.ctor;DefMgr.Set;";
static const char* const kMiddle1 =
    "Progress(Init Thing Components,-1,0,0);RegisterTCs;"
    "Profile(Init Definition Manager,0);Progress(Init Definition Manager,-1,0,0);InitDefinitions(1);"
    "Profile(Init Graphics,0);InitGraphics;InitialiseFonts;"
    "Progress(Init Subtitled Message,-1,0,0);SubtitledMessage.Initialise;"
    "Progress(Adding Console Variables,-1,0,0);Progress(Init Conversation Attitude,-1,0,0);ConversationAttitude.Init;"
    "Profile(Init Player Manager,0);Progress(Init Player Manager,-1,0,0);InitPlayerManager;"
    "Progress(Init Player Interface,-1,0,0);new(2200);GPI.ctor;";
static const char* const kDeleteCheckPoint = "DeleteCheckPointSave(flag=1);";
static const char* const kMiddle2 =
    "Progress(Init World,-1,0,0);InitWorld(flag140=A5);"
    "Profile(Init Display Engine,0);Progress(Init Display Engine,-1,0,0);InitDisplayEngine;"
    "Profile(Create Players,0);Progress(Create Players,-1,0,0);CreatePlayers;"
    "Profile(Init Sound,0);Progress(Init Sound,-1,0,0);InitSound;";
static const char* const kParticles =
    "Profile(Load Particles,0);Progress(Loading particles,-1,0,0);InitParticleEngine;";
static const char* const kPostInit = "PostInit;";
static const char* const kGameStart =
    "Smoother.Init(60,0);PeekWorldChecksum;Client.Update(111,333,222);PathExists(default_user.ini);";
static const char* const kDefaultScript = "RunScript(default_user.ini);";
static const char* const kUserScript = "RunScript(user.ini);";
static const char* const kTail = "GFGetSystemManager;GFGetTime;";

static bool Run(bool deleteCheckPoint, bool compileFrontend, bool allocated, bool configExists)
{
    ok = true;
    logText[0] = 0;
    liveStrings = 0;
    lastNew = 0;
    constructedInterface = 0;
    expectedReinit = 0;

    memset(storage, 0xA5, sizeof(storage));
    for (int i = 0; i < 10; ++i) fakeVtable[i] = (void*)&TrapSlot;
    fakeVtable[8] = (void*)&PostInitSlot;
    *reinterpret_cast<void***>(storage) = fakeVtable;
    *reinterpret_cast<void**>(storage + 0x20) = 0;                 // PPlayerInterface empty
    *reinterpret_cast<const CFontBank**>(storage + 0x16144) = fakeFont; // PDebugFont.Data
    *reinterpret_cast<long*>(storage + 0x48) = 0x111;              // CurrentServerFrame
    *reinterpret_cast<unsigned long*>(storage + 0x1613C) = 0x222;  // WorldSeed
    memcpy(before, storage, sizeof(storage));

    memset(worldStorage, 0xA5, sizeof(worldStorage));
    memset(systemManagerStorage, 0x5A, sizeof(systemManagerStorage));
    memset(displayManagerStorage, 0, sizeof(displayManagerStorage));
    *reinterpret_cast<long*>(displayManagerStorage + 0x1C8) = 60;  // RefreshRate
    g_FableConsolePClass_013CAA40.Data = reinterpret_cast<CConsole*>(consoleStorage);

    if (instanceSlot) *instanceSlot = 0;
    g_WorldFrame_013B7D70 = 0x1234;
    g_RenderFrame_013B7D6C = 0x5678;
    g_DeleteCheckPointSaveOnInitWorld_013B7C90 = deleteCheckPoint;
    g_FableCompileFrontendDefinitions_013B8648 = compileFrontend;
    definitionsAllocated = allocated;
    defaultConfigExists = configExists;

    trackNew = true;
    obj->CMainGameComponent::Init();
    trackNew = false;

    char expected[8192];
    strcpy(expected, kPrefix);
    if (!allocated) strcat(expected, kNewDefs);
    strcat(expected, kMiddle1);
    if (deleteCheckPoint) strcat(expected, kDeleteCheckPoint);
    strcat(expected, kMiddle2);
    if (!compileFrontend) strcat(expected, kParticles);
    strcat(expected, kPostInit);
    if (!compileFrontend)
    {
        strcat(expected, kGameStart);
        if (configExists) strcat(expected, kDefaultScript);
        strcat(expected, kUserScript);
    }
    strcat(expected, kTail);
    if (strcmp(expected, logText) != 0)
    {
        printf("FAIL: call log\n expected %s\n actual   %s\n", expected, logText);
        ok = false;
    }

    Expect(liveStrings == 0, "all strings destroyed");
    Expect(instanceSlot && *instanceSlot == obj, "PMainGameComponent");
    Expect(g_WorldFrame_013B7D70 == 0 && g_RenderFrame_013B7D6C == 0, "frame counters zeroed");
    Expect(g_DeleteCheckPointSaveOnInitWorld_013B7C90 == false, "checkpoint flag consumed");
    Expect(worldStorage[0x140] == (deleteCheckPoint ? 0 : 1), "world AutoSaveCheckPointExists");
    for (int i = 0; i < (int)sizeof(worldStorage); ++i)
        if (i != 0x140 && worldStorage[i] != 0xA5) { Expect(false, "world neighbour untouched"); break; }

    // System manager: only the reinit callback and its context change.
    Expect(*reinterpret_cast<void**>(systemManagerStorage + 0xF0) == (void*)expectedReinit, "AppReinitFunc");
    Expect(*reinterpret_cast<void**>(systemManagerStorage + 0xF4) == obj, "AppReinitContext");
    for (int i = 0; i < (int)sizeof(systemManagerStorage); ++i)
        if ((i < 0xF0 || i >= 0xF8) && systemManagerStorage[i] != 0x5A) { Expect(false, "system manager neighbour untouched"); break; }

    // Component fields: expected writes, everything else untouched.
    double duration = *reinterpret_cast<double*>(storage + 0x68);
    double wanted = 1.0 / 60.0 + 0.0001f;
    Expect(duration > wanted - 1e-9 && duration < wanted + 1e-9, "LastFrameRenderDuration");
    Expect(*reinterpret_cast<void**>(storage + 0x20) == constructedInterface && constructedInterface != 0, "PPlayerInterface");
    Expect(*reinterpret_cast<const CPlayerManager**>(storage + 0x1C) == fakePlayerManager, "PPlayerManager");
    Expect(*reinterpret_cast<unsigned char**>(storage + 0x24) == worldStorage, "PWorld");
    Expect(*reinterpret_cast<float*>(storage + 0x161B0) == 0.0f, "TimePassedSinceLastUpdate");
    Expect(*reinterpret_cast<float*>(storage + 0x161B4) == 12.5f, "LastUpdateTime");
    Expect(storage[0x161E0] == 1, "Initialised");
    for (int i = 0; i < (int)sizeof(storage); ++i)
    {
        bool written = (i >= 0x1C && i < 0x28) || (i >= 0x68 && i < 0x70) ||
            (i >= 0x161B0 && i < 0x161B8) || i == 0x161E0;
        if (!written && storage[i] != before[i]) { printf("FAIL: component byte +0x%X changed\n", i); ok = false; break; }
    }

    free(constructedInterface);
    return ok;
}

int main()
{
    bool pass = true;
    pass = Run(true, false, false, true) && pass;    // fresh game: delete checkpoint, new defs, both scripts
    pass = Run(false, false, true, false) && pass;   // checkpoint kept, defs exist, no default_user.ini
    pass = Run(false, true, true, true) && pass;     // compiling frontend defs: no particles, no client/scripts
    pass = Run(true, true, false, false) && pass;
    if (!pass) return 1;
    puts("FABLETLC_MAIN_GAME_INIT PASS");
    return 0;
}