#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_progress_display.h"

// Behaviour fixture for CMainGameComponent::UpdateRegular @ 0x004162B5.
// The component is a raw retail-sized buffer whose vptr is a fake table:
// slot 5 (+0x14) = Update, slot 7 (+0x1C) = Render. PDisplayEngine (+0x28)
// points at a CDisplayEngine-sized buffer whose Engine3D (+0x2C) points at a
// fake CIEngine whose slot 59 (+0xEC) = IsPreloaded.

class CThingSearchTools
{
public:
    static unsigned long NoLookups;
};
unsigned long CThingSearchTools::NoLookups;


double CMainGameComponent::ProfileUpdateTime;
double CMainGameComponent::ProfileRenderTime;

namespace
{
    int g_seq;
    double g_times[4];
    int g_timeCalls;
    int g_timeAt[4];

    unsigned char g_sysmgr[0x180];
    int g_sysAt;
    bool g_appActive;
    int g_appActiveAt;
    const void* g_appActiveThis;

    bool g_progress;
    int g_progressAt;

    bool g_preloaded;
    int g_preloadAt;
    const void* g_preloadThis;

    int g_updateAt;
    const void* g_updateThis;
    int g_renderAt;
    const void* g_renderThis;

    unsigned long g_lookupsAtUpdate;
}

double FABLE_FASTCALL GFGetTime()
{
    int call = g_timeCalls++;
    if (call < 4) g_timeAt[call] = ++g_seq;
    return call < 4 ? g_times[call] : -1.0;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    g_sysAt = ++g_seq;
    return reinterpret_cast<CSystemManager*>(g_sysmgr);
}

bool CSystemManager::IsApplicationActive()
{
    g_appActiveAt = ++g_seq;
    g_appActiveThis = this;
    return g_appActive;
}

bool FABLE_FASTCALL NProgressDisplay::IsActive()
{
    g_progressAt = ++g_seq;
    return g_progress;
}

static void __fastcall FakeUpdate(void* self, void*)
{
    g_updateAt = ++g_seq;
    g_updateThis = self;
    g_lookupsAtUpdate = CThingSearchTools::NoLookups;
}

static void __fastcall FakeRender(void* self, void*)
{
    g_renderAt = ++g_seq;
    g_renderThis = self;
}

static bool __fastcall FakeIsPreloaded(void* self, void*)
{
    g_preloadAt = ++g_seq;
    g_preloadThis = self;
    return g_preloaded;
}

static void __fastcall FakeTrap(void*, void*)
{
    printf("FAIL unexpected virtual\n");
}

static void* g_componentVtbl[10];
static void* g_engineVtbl[85];
static unsigned char g_component[0x161E8];
static unsigned char g_display[0x100];
static void* g_engine[3];

static bool RunCase(const char* name, bool active, bool progress, bool preloaded,
                    bool expectPreloadQuery, bool expectRender)
{
    g_seq = 0; g_timeCalls = 0;
    memset(g_timeAt, 0, sizeof(g_timeAt));
    g_sysAt = g_appActiveAt = g_progressAt = g_preloadAt = g_updateAt = g_renderAt = 0;
    g_appActiveThis = g_preloadThis = g_updateThis = g_renderThis = 0;
    g_lookupsAtUpdate = 0xFFFFFFFF;
    g_appActive = active; g_progress = progress; g_preloaded = preloaded;
    g_times[0] = 100.0; g_times[1] = 100.25; g_times[2] = 200.0; g_times[3] = 200.5;

    CThingSearchTools::NoLookups = 1234;
    CMainGameComponent::ProfileUpdateTime = -7.0;
    CMainGameComponent::ProfileRenderTime = -9.0;

    memset(g_component, 0xA5, sizeof(g_component));
    memset(g_display, 0x5A, sizeof(g_display));
    void* vt = g_componentVtbl;
    memcpy(g_component, &vt, 4);
    void* disp = g_display;
    memcpy(g_component + 0x28, &disp, 4);
    void* eng = g_engine;
    memcpy(g_display + 0x2C, &eng, 4);
    g_engine[0] = g_engineVtbl;
    g_engine[1] = g_engine[2] = (void*)0x11111111;

    reinterpret_cast<CMainGameComponent*>(g_component)->UpdateRegular();

    bool ok = g_sysAt == 1 && g_appActiveAt == 2 && g_appActiveThis == g_sysmgr;
    if (!active)
    {
        ok = ok && g_timeCalls == 0 && g_updateAt == 0 && g_renderAt == 0 && g_progressAt == 0
                && g_preloadAt == 0 && CThingSearchTools::NoLookups == 1234
                && CMainGameComponent::ProfileUpdateTime == -7.0
                && CMainGameComponent::ProfileRenderTime == -9.0;
    }
    else
    {
        ok = ok && g_timeCalls == 4 && CThingSearchTools::NoLookups == 0 && g_lookupsAtUpdate == 0;
        ok = ok && g_timeAt[0] == 3 && g_updateAt == 4 && g_updateThis == g_component
                && g_timeAt[1] == 5 && g_timeAt[2] == 6 && g_progressAt == 7;
        int next = 8;
        if (expectPreloadQuery)
            ok = ok && g_preloadAt == next++ && g_preloadThis == g_engine;
        else
            ok = ok && g_preloadAt == 0;
        if (expectRender)
            ok = ok && g_renderAt == next++ && g_renderThis == g_component;
        else
            ok = ok && g_renderAt == 0;
        ok = ok && g_timeAt[3] == next;
        ok = ok && CMainGameComponent::ProfileUpdateTime == 0.25
                && CMainGameComponent::ProfileRenderTime == 0.5;
    }
    for (int i = 4; i < (int)sizeof(g_component); ++i)
    {
        if (i >= 0x28 && i < 0x2C) continue;
        if (g_component[i] != 0xA5) { ok = false; break; }
    }
    for (int i = 0; i < (int)sizeof(g_display); ++i)
    {
        if (i >= 0x2C && i < 0x30) continue;
        if (g_display[i] != 0x5A) { ok = false; break; }
    }
    if (!ok) printf("FAIL %s\n", name);
    return ok;
}

int main()
{
    for (int i = 0; i < 10; ++i) g_componentVtbl[i] = (void*)&FakeTrap;
    for (int i = 0; i < 85; ++i) g_engineVtbl[i] = (void*)&FakeTrap;
    g_componentVtbl[5] = (void*)&FakeUpdate;
    g_componentVtbl[7] = (void*)&FakeRender;
    g_engineVtbl[59] = (void*)&FakeIsPreloaded;

    bool ok = true;
    ok &= RunCase("inactive", false, true, true, false, false);
    ok &= RunCase("no-progress", true, false, true, false, true);
    ok &= RunCase("progress-preloaded", true, true, true, true, false);
    ok &= RunCase("progress-not-preloaded", true, true, false, true, true);
    if (ok) printf("FABLETLC_MAINGAME_UPDATEREGULAR_PASS\n");
    return ok ? 0 : 1;
}