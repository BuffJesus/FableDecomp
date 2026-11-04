#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "test_support/import_redirect.h"
#include "fable_display_engine.h"
#include "fable_gfuninitialise.h"
#include "fable_world.h"

// Behaviour fixture for CMainGameComponent::UpdateForcedTickSpeed @ 0x00417747.
// The component is a raw retail-sized (0x161E8) buffer with a fake vtable
// (slot 6 = GetInputs); PWorld points at a buffer whose fake vtable slot 3 is
// GetThingManager. Every callee is a recording stand-in appending to g_log.
// Times are binary fractions so every expected sleep is exact. Sleep is
// reached through the import table (test_support/import_redirect.h).

class CThingSearchTools
{
public:
    static unsigned long NoLookups;
};
unsigned long CThingSearchTools::NoLookups;

long CMainGameComponent::ConstantFPS = 16;
unsigned long g_WorldFrame_013B7D70;

static char g_log[2048];
static void Log(const char* fmt, long v = 0)
{
    char buf[64];
    sprintf(buf, fmt, v);
    strcat(g_log, buf);
    strcat(g_log, ";");
}

static unsigned char g_component[0x161E8];
static unsigned char g_world[0x198];
static unsigned char g_display[0x100];
static unsigned char g_sysmgr[0x180];
static unsigned char g_thingManager[16];
static void* g_vtbl[10];
static void* g_worldVtbl[12];

static bool g_active, g_netSet;
static long g_frame;
static double g_times[8];
static int g_timeCalls;
static bool g_bad;
static void Check(bool ok) { if (!ok) g_bad = true; }

// What each RenderUpdate / Render call received.
static CInterpolationInfo g_seenInfo[4];
static const void* g_seenInfoPtr[4];
static int g_renders;
static bool g_seenTurn[4];

static void __stdcall FakeSleep(unsigned long ms) { Log("sleep%ld", (long)ms); }

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Log("sys");
    return reinterpret_cast<CSystemManager*>(g_sysmgr);
}

bool CSystemManager::IsApplicationActive()
{
    Check((void*)this == g_sysmgr);
    Log("active");
    return g_active;
}

double FABLE_FASTCALL GFGetTime()
{
    Log("time");
    return g_times[g_timeCalls++];
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

static void __fastcall FakeGetInputs(void* self, void*)
{
    Check(self == g_component);
    Log("inputs");
}

void CMainGameComponent::SignalFrameUpdate()
{
    Check((void*)this == g_component);
    Log("signal");
}

void CMainGameComponent::UpdateFromEventPackageSet(const CGameEventPackageSet* set)
{
    Check((void*)this == g_component && (const void*)set == g_component + 0xA4);
    Log("updfrom");
}

long FABLE_FASTCALL CWorld::GetFrame()
{
    Log("frame");
    return g_frame;
}

static void* __fastcall FakeGetThingManager(void* self, void*)
{
    Check(self == g_world);
    Log("thingmgr");
    return g_thingManager;
}

void FABLE_FASTCALL CThing::SetCurrentThingManager(CThingManager& manager)
{
    Check((void*)&manager == g_thingManager);
    Log("setthing");
}

CInterpolationInfo::CInterpolationInfo()
{
    // The retail constructor (0x00415A60) zeroes all 13 dwords; the stand-in
    // marks the members the caller must leave alone.
    BulletTimeCurrent.GTPredictedTimeSinceLastRenderFrame = 7.0f;
    BulletTimeCurrent.WFInterpolate = 8.0f;
    BulletTimeLast.GTPredictedTimeSinceLastRenderFrame = 9.0f;
    BulletTimeLast.WFInterpolate = 10.0f;
    Log("ictor");
}

void CRoughFrameCounter::NewFrame()
{
    Check((void*)this == g_component + 0x161BC);
    Log("newframe");
}

double CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const
{
    Check((const void*)this == g_component);
    Log("predicted");
    return 0.25 + g_renders;
}

void CWorld::RenderUpdate(const CInterpolationInfo* info)
{
    Check((void*)this == g_world && g_renders < 4);
    g_seenInfo[g_renders] = *info;
    g_seenInfoPtr[g_renders] = info;
    Log("renderupd");
}

double CMainGameComponent::GetCurrentGameTime() const
{
    Check((const void*)this == g_component);
    Log("gametime");
    return 100.0 + g_renders;
}

void CDisplayEngine::Render(const CInterpolationInfo* info, bool worldUpdated, bool present)
{
    Check((void*)this == g_display && info == g_seenInfoPtr[g_renders] && present);
    g_seenTurn[g_renders] = worldUpdated;
    ++g_renders;
    Log("render");
}


static void __fastcall FakeTrap(void*, void*) { Log("TRAP"); }

template<typename T> static void Put(int off, T v) { memcpy(g_component + off, &v, sizeof(T)); }
template<typename T> static T Get(int off) { T v; memcpy(&v, g_component + off, sizeof(T)); return v; }

static bool Written(int i)
{
    static const int spans[][2] = {
        { 0x24, 8 }, { 0x38, 12 }, { 0x48, 4 }, { 0x68, 8 }, { 0x70, 0x34 },
        { 0x16138, 4 }, { 0x161A8, 8 }, { 0x161B8, 1 } };
    for (int k = 0; k < (int)(sizeof(spans) / sizeof(spans[0])); ++k)
        if (i >= spans[k][0] && i < spans[k][0] + spans[k][1])
            return true;
    return false;
}

struct Case
{
    const char* name;
    bool active, netSet; long frame;
    float multiplier, desired;
    int noTimes; double times[8];
    const char* expect;
    long expectFailed; int expectRenders; bool expectTurn; unsigned long expectNoRender;
};

static bool Run(const Case& c)
{
    g_log[0] = 0; g_bad = false; g_timeCalls = 0; g_renders = 0;
    memset(g_seenInfo, 0, sizeof(g_seenInfo));
    memcpy(g_times, c.times, sizeof(g_times));
    g_active = c.active; g_netSet = c.netSet; g_frame = c.frame;
    CThingSearchTools::NoLookups = 55;
    g_WorldFrame_013B7D70 = 1;

    memset(g_component, 0xA5, sizeof(g_component));
    void* vt = g_vtbl; memcpy(g_component, &vt, 4);
    void* wvt = g_worldVtbl; memcpy(g_world, &wvt, 4);
    Put<void*>(0x24, g_world);
    Put<void*>(0x28, g_display);
    Put<float>(0x38, c.multiplier);
    Put<float>(0x3C, c.desired);
    Put<long>(0x40, 3);
    Put<long>(0x48, 77);
    Put<double>(0x68, -1.0);
    // LastInterpolationInfo: Current {1,2}, Last {3,4}, PausedCurrent {5,6}, ...
    for (int k = 0; k < 12; ++k) Put<float>(0x70 + 4 * k, (float)(k + 1));
    Put<long>(0x70 + 0x30, 99);
    Put<unsigned long>(0x16138, 5);
    Put<double>(0x161A8, -2.0);
    Put<bool>(0x161B8, false);

    static_cast<CMainGameComponent*>((void*)g_component)->UpdateForcedTickSpeed();

    bool ok = strcmp(g_log, c.expect) == 0 && !g_bad;
    ok = ok && g_timeCalls == c.noTimes && g_renders == c.expectRenders;
    ok = ok && Get<long>(0x40) == c.expectFailed;
    ok = ok && Get<unsigned long>(0x16138) == c.expectNoRender;
    ok = ok && Get<bool>(0x161B8) == c.expectTurn;
    if (c.active)
        ok = ok && CThingSearchTools::NoLookups == 0 && g_WorldFrame_013B7D70 == (unsigned long)c.frame;
    else
        ok = ok && CThingSearchTools::NoLookups == 55 && g_WorldFrame_013B7D70 == 1;

    // Interpolation info handed to RenderUpdate and Render.
    float prevCurrent[2] = { 1.0f, 2.0f }, prevPaused[2] = { 5.0f, 6.0f };
    for (int r = 0; r < g_renders; ++r)
    {
        const CInterpolationInfo& info = g_seenInfo[r];
        float interp = r * (1.0f / (c.desired / c.multiplier / (16.0f / c.multiplier)));
        ok = ok && info.Last.GTPredictedTimeSinceLastRenderFrame == prevCurrent[0]
                && info.Last.WFInterpolate == prevCurrent[1]
                && info.PausedLast.GTPredictedTimeSinceLastRenderFrame == prevPaused[0]
                && info.PausedLast.WFInterpolate == prevPaused[1];
        ok = ok && info.Current.GTPredictedTimeSinceLastRenderFrame == (float)(0.25 + r)
                && info.Current.WFInterpolate == interp
                && info.PausedCurrent.GTPredictedTimeSinceLastRenderFrame == info.Current.GTPredictedTimeSinceLastRenderFrame
                && info.PausedCurrent.WFInterpolate == interp;
        ok = ok && info.BulletTimeCurrent.GTPredictedTimeSinceLastRenderFrame == 7.0f
                && info.BulletTimeLast.WFInterpolate == 10.0f && info.WFServerCurrentTime == 77;
        ok = ok && g_seenTurn[r] == c.expectTurn;
        prevCurrent[0] = prevPaused[0] = info.Current.GTPredictedTimeSinceLastRenderFrame;
        prevCurrent[1] = prevPaused[1] = info.Current.WFInterpolate;
    }
    if (g_renders > 0)
    {
        // LastInterpolationInfo is the last info rendered; timing fields updated.
        ok = ok && memcmp(g_component + 0x70, &g_seenInfo[g_renders - 1], 0x34) == 0;
        ok = ok && Get<double>(0x161A8) == 100.0 + (g_renders - 1);
        ok = ok && Get<double>(0x68) == c.times[2 * g_renders + 1] - c.times[2 * g_renders];
    }
    else
    {
        ok = ok && Get<double>(0x161A8) == -2.0 && Get<double>(0x68) == -1.0 && Get<float>(0x70) == 1.0f;
    }
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
    for (int i = 0; i < 12; ++i) g_worldVtbl[i] = (void*)&FakeTrap;
    g_vtbl[6] = (void*)&FakeGetInputs;
    g_worldVtbl[3] = (void*)&FakeGetThingManager;

    // ConstantFPS 16: turnTime 1/16. Update takes 1/128 (start 1.0).
    static const Case cases[] = {
        // desired 32 -> 2 renders per turn, slice (1/16-1/128)/2 = 0.02734375;
        // each render takes 1/128 -> sleep 19.53125 ms -> 19; turn exactly used.
        { "two-renders", true, true, 1234, 1.0f, 32.0f,
          6, { 1.0, 1.0078125, 2.0, 2.0078125, 3.0, 3.0078125 },
          "sys;active;time;netset;interp;inputs;signal;updfrom;frame;time;frame;thingmgr;setthing;"
          "time;ictor;newframe;predicted;renderupd;gametime;render;time;sleep19;"
          "time;ictor;newframe;predicted;renderupd;gametime;render;time;sleep19;sleep0;",
          3, 2, true, 2 },
        // desired 8 -> framesPerTurn 0.5 -> one render, slice 0.109375, sleep
        // 101.5625 -> 101; time used 0.1171875 > 1/16 -> a failed update.
        { "overrun", true, true, 1234, 1.0f, 8.0f,
          4, { 1.0, 1.0078125, 2.0, 2.0078125 },
          "sys;active;time;netset;interp;inputs;signal;updfrom;frame;time;frame;thingmgr;setthing;"
          "time;ictor;newframe;predicted;renderupd;gametime;render;time;sleep101;sleep0;",
          4, 1, true, 1 },
        // desired 0.25 -> slice 3.5 s -> sleep clamps to 1000.
        { "clamp-high", true, false, 1234, 1.0f, 0.25f,
          4, { 1.0, 1.0078125, 2.0, 2.0078125 },
          "sys;active;time;netset;frame;time;frame;thingmgr;setthing;"
          "time;ictor;newframe;predicted;renderupd;gametime;render;time;sleep1000;sleep0;",
          4, 1, false, 6 },
        // multiplier 2 scales both rates: game fps 8 (turn 1/8), desired 8 ->
        // one render; slice 0.1171875 - 1/128 -> 109.375 -> 109; turn used exactly.
        { "multiplier", true, true, 1234, 2.0f, 16.0f,
          4, { 1.0, 1.0078125, 2.0, 2.0078125 },
          "sys;active;time;netset;interp;inputs;signal;updfrom;frame;time;frame;thingmgr;setthing;"
          "time;ictor;newframe;predicted;renderupd;gametime;render;time;sleep109;sleep0;",
          3, 1, true, 1 },
        // No world frame yet: no rendering; sleep off (1/16 - 1/128) = 54.6875 -> 54.
        { "no-world-frame", true, true, 0, 1.0f, 32.0f,
          2, { 1.0, 1.0078125 },
          "sys;active;time;netset;interp;inputs;signal;updfrom;frame;time;frame;sleep54;",
          3, 0, true, 0 },
        // Update overran the whole turn: remaining sleep clamps to 0.
        { "slow-update", true, false, 0, 1.0f, 32.0f,
          2, { 1.0, 1.5 },
          "sys;active;time;netset;frame;time;frame;sleep0;",
          3, 0, false, 5 },
        { "inactive", false, true, 1234, 1.0f, 32.0f,
          0, { 0.0 },
          "sys;active;",
          3, 0, false, 5 },
    };
    bool ok = true;
    for (int i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); ++i)
        ok &= Run(cases[i]);
    if (ok) printf("FABLETLC_MAINGAME_UPDATEFORCEDTICKSPEED_PASS\n");
    return ok ? 0 : 1;
}