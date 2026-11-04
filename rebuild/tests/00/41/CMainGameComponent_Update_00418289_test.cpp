#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "test_support/import_redirect.h"
#include "fable_game_event.h"
#include "fable_nui_manager.h"
#include "fable_player_gui.h"
#include "fable_system_manager.h"
#include "fable_world.h"

// Behaviour fixture for CMainGameComponent::Update @ 0x00418289.
// The component is a raw retail-sized (0x161E8) buffer with a fake vtable
// (slot 6 = GetInputs). Every callee is a recording stand-in that appends a
// token to g_log; each case compares the full call trace, the arguments and
// receivers, and the field writes (neighbours must stay 0xA5).
// Sleep is reached through the import table (test_support/import_redirect.h).

static unsigned char g_gui[0x140];

// The CPlayerGui singleton points at g_gui. CCountedPointer's copy constructor
// is out of line in retail (per-T); the fixture supplies the plain copy.
template<> CCountedPointer<CPlayerGui>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
}

static CCountedPointer<CPlayerGui> MakeGuiPointer()
{
    CCountedPointer<CPlayerGui> p;
    p.Data = reinterpret_cast<CPlayerGui*>(g_gui);
    return p;
}

template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass = MakeGuiPointer();

// Update is protected: reach it through a derived type (a qualified, direct call).
class UpdateCaller : public CMainGameComponent
{
public:
    static void Call(CMainGameComponent* component)
    {
        static_cast<UpdateCaller*>(component)->CMainGameComponent::Update();
    }
};

long g_DelayEachFrameMS_013B8610;
unsigned long g_WorldFrame_013B7D70;
long g_ForceRenderFramesPerSec_013B8630;
CInputManager* g_FableInputManager_013B8388;

static char g_log[1024];
static void Log(const char* fmt, long v = 0, long w = 0)
{
    char buf[64];
    sprintf(buf, fmt, v, w);
    strcat(g_log, buf);
    strcat(g_log, ";");
}

static unsigned char g_component[0x161E8];
static unsigned char g_world[0x198];
static unsigned char g_manager[0xD0];
static unsigned char g_input[0x84];
static void* g_vtbl[10];

static bool g_disc, g_inter, g_timeForUpdate, g_fromSave, g_netSet;
static long g_diff;
static double g_times[2];
static int g_timeCalls;
static bool g_badThis;

static void Check(bool ok) { if (!ok) g_badThis = true; }

static void __stdcall FakeSleep(unsigned long ms) { Log("sleep%ld", (long)ms); }

double FABLE_FASTCALL GFGetTime()
{
    Log("time");
    return g_times[g_timeCalls++ & 1];
}

bool CMainGameComponent::IsControllerDisconnected() const
{
    Check((const void*)this == g_component);
    Log("disc");
    return g_disc;
}

bool CSubtitleRenderer::IsInInteractiveState() const
{
    Check((const void*)this == g_gui + 0x18);
    Log("inter");
    return g_inter;
}

NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance()
{
    Log("inst");
    return reinterpret_cast<NUISystem::CManager*>(g_manager);
}

void NUISystem::CManager::ClearErrorMessage()
{
    Check((void*)this == g_manager);
    Log("clear");
}

void FABLE_FASTCALL ShowErrorMessage(ERRORMESSAGE_TYPE type)
{
    Log("show%ld", (long)type);
}

static const CGameEvent* g_lastEvent;
void CGameEvent::AddData(long size, const void* data)
{
    Log("data%ld:%ld", size, *(const unsigned char*)data);
    g_lastEvent = this;
    Data[0] = *(const unsigned char*)data;   // stand-in so AddGameEvent can see it
}

void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{
    Check((void*)this == g_component && &event == g_lastEvent);
    Check(event.Type == 9 && event.Player == -1 && event.EndPos == 0 && event.Valid && !event.Replacement);
    Log("addevt%ld", event.Data[0]);
}

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{
    Check((const void*)this == g_component);
    Log("istime%ld", frame);
    return g_timeForUpdate;
}

bool CMainGameComponent::GetEventPackageSetFromSave(CGameEventPackageSet* set)
{
    Check((void*)this == g_component && (void*)set == g_component + 0xA4);
    Log("fromsave");
    return g_fromSave;
}

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* set)
{
    Check((void*)this == g_component && (const void*)set == g_component + 0xA4);
    Log("updfrom");
}

void CMainGameComponent::AddEventPackageSetToSave(const CGameEventPackageSet* set)
{
    Check((void*)this == g_component && (const void*)set == g_component + 0xA4);
    Log("tosave");
}

void CMainGameComponent::SignalFrameUpdate()
{
    Check((void*)this == g_component);
    Log("signal");
}

long CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const
{
    Check((const void*)this == g_component);
    Log("diff%ld", frame);
    return g_diff;
}

bool CNetworkClient::GetGameEventPackageSet(CGameEventPackageSet* set)
{
    Check((void*)this == g_component + 0x13AB8 && (void*)set == g_component + 0xA4);
    Log("netset");
    return g_netSet;
}

void CWorld::UpdateInputRelatedInterpolation()
{
    Check((void*)this == g_world);
    Log("interp");
}

long FABLE_FASTCALL CWorld::GetFrame()
{
    Log("frame");
    return 4242;
}

static void __fastcall FakeGetInputs(void* self, void*)
{
    Check(self == g_component);
    Log("inputs");
}

static void __fastcall FakeTrap(void*, void*) { Log("TRAP"); }


struct Case
{
    const char* name;
    long delay; bool disc, inter, error, paused;
    bool loading, saving, timeForUpdate, fromSave;
    bool local; long frame; bool inLoading, inSaving; long diff; long force; bool netSet;
    const char* expect;
    long expectFrame; bool expectAllowRender; bool expectTurn; bool expectNoRenderReset;
};

template<typename T> static void Put(int off, T v) { memcpy(g_component + off, &v, sizeof(T)); }
template<typename T> static T Get(int off) { T v; memcpy(&v, g_component + off, sizeof(T)); return v; }

static bool Written(int i)
{
    static const int spans[][2] = {
        { 0x24, 4 }, { 0x48, 4 }, { 0x1611A, 1 }, { 0x16138, 4 }, { 0x16170, 2 },
        { 0x161B0, 9 }, { 0x161E1, 1 } };
    for (int k = 0; k < (int)(sizeof(spans) / sizeof(spans[0])); ++k)
        if (i >= spans[k][0] && i < spans[k][0] + spans[k][1])
            return true;
    return false;
}

static bool Run(const Case& c)
{
    g_log[0] = 0; g_badThis = false; g_timeCalls = 0;
    g_times[0] = 10.75; g_times[1] = 11.5;
    memset(g_component, 0xA5, sizeof(g_component));
    void* vt = g_vtbl; memcpy(g_component, &vt, 4);
    Put<void*>(0x24, g_world);
    Put<long>(0x48, c.frame);
    Put<bool>(0x1611A, c.local);
    Put<unsigned long>(0x16138, 77);
    Put<bool>(0x16170, c.loading);
    Put<bool>(0x16171, c.saving);
    Put<float>(0x161B4, 10.5f);
    Put<bool>(0x161B8, true);
    Put<bool>(0x161E1, true);
    g_world[0xA4] = c.paused;
    g_manager[0x9C] = c.error;
    g_input[0x38] = c.inLoading; g_input[0x39] = c.inSaving;
    g_DelayEachFrameMS_013B8610 = c.delay;
    g_ForceRenderFramesPerSec_013B8630 = c.force;
    g_WorldFrame_013B7D70 = 1;
    g_disc = c.disc; g_inter = c.inter; g_timeForUpdate = c.timeForUpdate; g_fromSave = c.fromSave;
    g_diff = c.diff; g_netSet = c.netSet;

    UpdateCaller::Call(reinterpret_cast<CMainGameComponent*>(g_component));

    bool ok = strcmp(g_log, c.expect) == 0 && !g_badThis;
    ok = ok && Get<float>(0x161B0) == 0.25f && Get<float>(0x161B4) == 11.5f;
    ok = ok && Get<bool>(0x161B8) == c.expectTurn;
    ok = ok && Get<unsigned long>(0x16138) == (c.expectNoRenderReset ? 0UL : 77UL);
    ok = ok && Get<long>(0x48) == c.expectFrame;
    ok = ok && g_component[0x161E1] == (c.expectAllowRender ? 1 : 0);
    ok = ok && g_WorldFrame_013B7D70 == 4242;
    for (int i = 4; i < (int)sizeof(g_component); ++i)
    {
        if (Written(i)) continue;
        if (g_component[i] != 0xA5) { ok = false; printf("byte %x\n", i); break; }
    }
    if (!ok) printf("FAIL %s\n  got %s\n  exp %s\n", c.name, g_log, c.expect);
    return ok;
}

int main()
{
    FableTest::ImportRedirect sleepRedirect("Sleep", (void*)&FakeSleep);
    if (!sleepRedirect.Installed()) { printf("FAIL could not redirect Sleep\n"); return 1; }
    for (int i = 0; i < 10; ++i) g_vtbl[i] = (void*)&FakeTrap;
    g_vtbl[6] = (void*)&FakeGetInputs;
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(g_input);
    if ((void*)&CPlayerGui::Get() != g_gui) { printf("FAIL singleton\n"); return 1; }

    // The static BRequireUnpause carries between cases: A sets it, D (world
    // already paused) clears it so E must not unpause; A sets it again and B
    // consumes it (unpause event + clear).
    static const Case cases[] = {
        { "A pause+save-tick", 5, true, true, false, false,  true, false, true, true,
          false, 3, false, false, 0, 0, false,
          "sleep5;disc;inter;inst;data1:1;addevt1;show0;time;time;istime3;fromsave;updfrom;signal;frame;",
          3, true, true, true },
        { "D paused-world", 0, true, true, false, true,  true, false, true, false,
          false, 3, false, false, 0, 0, false,
          "disc;inter;inst;show0;time;time;istime3;fromsave;signal;frame;",
          3, true, false, false },
        { "E clear-no-unpause", 0, false, true, true, false,  false, false, false, false,
          false, 5, false, false, 20, 0, false,
          "disc;inst;inst;clear;time;time;netset;frame;",
          5, true, false, false },
        { "A pause+save-tick", 5, true, true, false, false,  true, false, true, true,
          false, 3, false, false, 0, 0, false,
          "sleep5;disc;inter;inst;data1:1;addevt1;show0;time;time;istime3;fromsave;updfrom;signal;frame;",
          3, true, true, true },
        { "B unpause+net-catchup", 0, false, false, true, false,  false, true, false, false,
          true, 5, false, false, 20, 0, true,
          "disc;inst;inst;clear;data1:0;addevt0;time;time;diff5;netset;interp;inputs;signal;tosave;updfrom;frame;",
          25, false, true, true },
        { "C error-already-shown", 0, true, true, true, false,  true, false, false, false,
          false, 3, false, false, 0, 0, false,
          "disc;inter;inst;time;time;istime3;frame;",
          3, true, false, false },
        { "F not-interactive frame1", 0, true, false, false, false,  false, false, false, false,
          true, 1, false, false, 20, 0, true,
          "disc;inter;inst;time;time;netset;interp;inputs;signal;updfrom;frame;",
          1, true, true, true },
        { "G input-loading", 0, false, false, false, false,  false, false, false, false,
          true, 5, true, false, 20, 0, false,
          "disc;inst;time;time;netset;frame;", 5, true, false, false },
        { "H input-saving", 0, false, false, false, false,  false, false, false, false,
          true, 5, false, true, 20, 0, false,
          "disc;inst;time;time;netset;frame;", 5, true, false, false },
        { "I diff15", 0, false, false, false, false,  false, false, false, false,
          true, 5, false, false, 15, 0, false,
          "disc;inst;time;time;diff5;netset;frame;", 5, true, false, false },
        { "J forced-fps", 0, false, false, false, false,  false, false, false, false,
          true, 5, false, false, 20, 30, false,
          "disc;inst;time;time;diff5;netset;frame;", 5, true, false, false },
        { "K negative-delay", -3, false, false, false, false,  true, false, true, true,
          false, 9, false, false, 0, 0, false,
          "disc;inst;time;time;istime9;fromsave;updfrom;signal;frame;", 9, true, true, true },
    };
    bool ok = true;
    for (int i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); ++i)
        ok &= Run(cases[i]);
    if (ok) printf("FABLETLC_MAINGAME_UPDATE_PASS\n");
    return ok ? 0 : 1;
}