#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_world.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CDisplayEngine.h"

#define CHECK(x) do { if (!(x)) { printf("check failed at line %d\n", __LINE__); ok = false; } } while (0)

// Fixture for CMainGameComponent::Render 0x00417001. Every callee is a
// recording stand-in; CWorld's vtable is a fake whose slot 3 (GetThingManager)
// records its receiver. Private members are reached by retail offset.
namespace
{
    enum Kind
    {
        EV_CTOR = 1, EV_SET_THING_MANAGER, EV_GET_SYSTEM, EV_IS_ACTIVE, EV_GET_FRAME,
        EV_GET_THING_MANAGER, EV_FORCE_RENDER, EV_NEXT_PRESENT, EV_LIMIT, EV_PREDICTED,
        EV_BT_ACTIVE, EV_BT_CONVERT, EV_RENDER_UPDATE, EV_FREE_TO_RENDER, EV_TIME, EV_RENDER
    };

    struct Event
    {
        int kind;
        const void* self;
        const void* arg;
        double value;
        double lo;
        double hi;
        bool flag1;
        bool flag2;
        CInterpolationInfo info;
        double lastRenderStart;
    };

    Event g_log[64];
    int g_count;

    long* g_constantFps;
    unsigned char* g_component;
    unsigned char g_systemStorage[0x200];
    unsigned char g_thingManagerStorage[4];
    CSystemManager* const g_system = reinterpret_cast<CSystemManager*>(g_systemStorage);
    CThingManager* const g_thingManager = reinterpret_cast<CThingManager*>(g_thingManagerStorage);

    bool g_active;
    long g_frame;
    double g_nextPresent;
    double g_predicted;
    bool g_bulletTime;
    bool g_free[2];
    int g_freeIndex;
    double g_times[3];
    int g_timeIndex;

    Event& Log(int kind, const void* self, const void* arg)
    {
        static Event overflow;
        if (g_count >= 64) { ++g_count; return overflow; }
        Event& e = g_log[g_count++];
        memset(&e, 0, sizeof(e));
        e.kind = kind; e.self = self; e.arg = arg;
        return e;
    }

    long* RegisterPrivates(long* constantFps)
    {
        g_constantFps = constantFps;
        return constantFps;
    }

    struct Probe : CMainGameComponent
    {
        static void Call(CMainGameComponent* component)
        {
            static_cast<Probe*>(component)->CMainGameComponent::Render();
        }
    };

    template<class T> T Get(unsigned long offset)
    {
        T value; memcpy(&value, g_component + offset, sizeof(T)); return value;
    }
    template<class T> void Put(unsigned long offset, T value)
    {
        memcpy(g_component + offset, &value, sizeof(T));
    }

    bool SameSet(const CInterpolationInfoSet& a, float gt, float wf)
    {
        return a.GTPredictedTimeSinceLastRenderFrame == gt && a.WFInterpolate == wf;
    }

    CThingManager& FABLE_FASTCALL FakeGetThingManager(void* self, void*)
    {
        Log(EV_GET_THING_MANAGER, self, 0);
        return *g_thingManager;
    }
}

long CMainGameComponent::ConstantFPS = (RegisterPrivates(&ConstantFPS), 16L);
bool CMainGameComponent::AlwaysUpdateGameFrame;
long g_ForceRenderFramesPerSec_013B8630;
unsigned long g_RenderFrame_013B7D6C;

CInterpolationInfo::CInterpolationInfo() : WFServerCurrentTime(0)
{
    Log(EV_CTOR, this, 0);
}

void FABLE_FASTCALL CThing::SetCurrentThingManager(CThingManager& manager)
{
    Log(EV_SET_THING_MANAGER, 0, &manager);
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Log(EV_GET_SYSTEM, 0, 0);
    return g_system;
}

bool CSystemManager::IsApplicationActive()
{
    Log(EV_IS_ACTIVE, this, 0);
    return g_active;
}

long FABLE_FASTCALL CWorld::GetFrame()
{
    Log(EV_GET_FRAME, 0, 0);
    return g_frame;
}

void CMainGameComponent::ForceRender(long framesPerSecond)
{
    Log(EV_FORCE_RENDER, this, 0).value = framesPerSecond;
}

double CMainGameComponent::GetGameTimeOfNextPresentCompletion() const
{
    Log(EV_NEXT_PRESENT, this, 0);
    return g_nextPresent;
}

template<>
const double& FABLE_FASTCALL GFLimitReturn<double>(const double& value, const double& minimum,
                                                   const double& maximum)
{
    Event& e = Log(EV_LIMIT, 0, 0);
    e.value = value; e.lo = minimum; e.hi = maximum;
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

double CMainGameComponent::GetPredictedTimeSinceLastRenderFrame() const
{
    Log(EV_PREDICTED, this, 0);
    return g_predicted;
}

bool CBulletTimeManager::IsBulletTimeActive() const
{
    Log(EV_BT_ACTIVE, this, 0);
    return g_bulletTime;
}

float CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate(float interpolate) const
{
    Log(EV_BT_CONVERT, this, 0).value = interpolate;
    return interpolate * 2.0f + 1.0f;
}

void CWorld::RenderUpdate(const CInterpolationInfo* info)
{
    Log(EV_RENDER_UPDATE, this, info).info = *info;
}

bool CNetworkClient::IsFreeToRender() const
{
    Log(EV_FREE_TO_RENDER, this, 0);
    return g_free[g_freeIndex++ & 1];
}

double CMainGameComponent::GetCurrentGameTime() const
{
    Log(EV_TIME, this, 0);
    return g_times[g_timeIndex++ % 3];
}

void CDisplayEngine::Render(const CInterpolationInfo* info, bool worldUpdateTurn, bool present)
{
    Event& e = Log(EV_RENDER, this, info);
    e.flag1 = worldUpdateTurn;
    e.flag2 = present;
    e.info = *info;
    e.lastRenderStart = Get<double>(0x161A8);
}

namespace
{
    unsigned char g_worldStorage[sizeof(CWorld)];
    unsigned char g_engineStorage[sizeof(CDisplayEngine)];
    unsigned char g_bulletStorage[sizeof(CBulletTimeManager)];
    void* g_worldVtable[12];

    CWorld* World() { return reinterpret_cast<CWorld*>(g_worldStorage); }
    CDisplayEngine* Engine() { return reinterpret_cast<CDisplayEngine*>(g_engineStorage); }
    CBulletTimeManager* Bullet() { return reinterpret_cast<CBulletTimeManager*>(g_bulletStorage); }
    CMainGameComponent* Self() { return reinterpret_cast<CMainGameComponent*>(g_component); }
    const void* Client() { return g_component + 0x13AB8; }

    void Reset()
    {
        memset(g_component, 0, sizeof(CMainGameComponent));
        memset(g_worldStorage, 0, sizeof(g_worldStorage));
        memset(g_engineStorage, 0, sizeof(g_engineStorage));
        for (int s = 0; s < 12; ++s)
            g_worldVtable[s] = 0;
        g_worldVtable[3] = (void*)&FakeGetThingManager;
        void* vtable = g_worldVtable;
        memcpy(g_worldStorage, &vtable, sizeof(vtable));
        CBulletTimeManager* bullet = Bullet();
        memcpy(g_worldStorage + 0x68, &bullet, sizeof(bullet));
        Engine()->Frame = 1234;

        Put<void*>(0x24, World());
        Put<void*>(0x28, Engine());
        Put<long>(0x48, 51);                   // CurrentServerFrame
        Put<double>(0x60, 77.0);               // GameStartTime (untouched)
        Put<double>(0x68, -1.0);               // LastFrameRenderDuration
        for (int i = 0; i < 12; ++i)           // LastInterpolationInfo sentinels
            Put<float>(0x70 + 4 * i, 100.0f + i);
        Put<long>(0xA0, 999);
        Put<unsigned long>(0x16138, 20);       // NoRenderFramesSinceLastGameUpdate
        Put<double>(0x161A8, -2.0);            // LastRenderFrameStartTime
        Put<unsigned char>(0x161B8, 1);        // WorldUpdateTurn
        Put<unsigned char>(0x161E1, 1);        // AllowRender
        Put<unsigned char>(0x161E2, 0);        // Rendered
        Put<unsigned char>(0x161E3, 0x5A);     // padding neighbour
        Put<long>(0x161E4, 4);                 // DebugNoFramesUnableToRender
        Put<unsigned long>(0x16134, 0xDEADBEEF);

        g_count = 0;
        g_active = true;
        g_frame = 2;
        g_nextPresent = 103.0 / 32.0;          // * 16 = 51.5 -> interpolate 0.5
        g_predicted = 0.125;
        g_bulletTime = false;
        g_free[0] = g_free[1] = true;
        g_freeIndex = 0;
        g_times[0] = 10.0; g_times[1] = 10.5; g_times[2] = 12.25;
        g_timeIndex = 0;
        g_ForceRenderFramesPerSec_013B8630 = 0;
        g_RenderFrame_013B7D6C = 0;
        CMainGameComponent::AlwaysUpdateGameFrame = false;
    }

    // Shared prefix of every path: ctor, thing manager, active check.
    bool Prefix(const void** info)
    {
        *info = g_log[0].self;
        return g_count >= 5 && g_log[0].kind == EV_CTOR &&
            g_log[1].kind == EV_GET_THING_MANAGER && g_log[1].self == World() &&
            g_log[2].kind == EV_SET_THING_MANAGER && g_log[2].arg == g_thingManager &&
            g_log[3].kind == EV_GET_SYSTEM &&
            g_log[4].kind == EV_IS_ACTIVE && g_log[4].self == g_system;
    }

    bool Untouched()
    {
        bool same = true;
        for (int i = 0; i < 12; ++i)
            same = same && Get<float>(0x70 + 4 * i) == 100.0f + i;
        return same && Get<unsigned char>(0x161E2) == 0 && Get<double>(0x68) == -1.0 &&
            Get<unsigned long>(0x16138) == 20 && Get<long>(0x161E4) == 4;
    }

    // Checks the render tail starting at g_log[at] (Render + timing updates).
    bool RenderTail(int at, const void* info)
    {
        const Event* e = &g_log[at];
        return e[0].kind == EV_TIME && e[0].self == Self() &&
            e[1].kind == EV_TIME && e[1].self == Self() &&
            e[2].kind == EV_RENDER && e[2].self == Engine() && e[2].arg == info &&
            e[2].flag1 == true && e[2].flag2 == true && e[2].lastRenderStart == 10.5 &&
            e[3].kind == EV_TIME && e[3].self == Self() &&
            Get<double>(0x68) == 12.25 - 10.0 && Get<double>(0x161A8) == 10.5 &&
            Get<unsigned long>(0x16138) == 21 && Get<long>(0x161E4) == 0 &&
            Get<unsigned char>(0x161E2) == 1;
    }
}

int main()
{
    g_component = static_cast<unsigned char*>(malloc(sizeof(CMainGameComponent)));
    bool ok = true;
    const void* info = 0;

    // 1. Application inactive: only the prefix, then GRenderFrame.
    Reset();
    g_active = false;
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 5);
    CHECK(Untouched() && g_RenderFrame_013B7D6C == 1234);

    // 2. Rendering not allowed: GetFrame is never asked.
    Reset();
    Put<unsigned char>(0x161E1, 0);
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 5 && Untouched() && g_RenderFrame_013B7D6C == 1234);

    // 3. World frame <= 1: nothing rendered.
    Reset();
    g_frame = 1;
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 6 && g_log[5].kind == EV_GET_FRAME);
    CHECK(Untouched() && g_RenderFrame_013B7D6C == 1234);

    // 4. Forced render below ConstantFPS is raised to it.
    Reset();
    g_ForceRenderFramesPerSec_013B8630 = 10;
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 7 && g_log[5].kind == EV_GET_FRAME &&
          g_log[6].kind == EV_FORCE_RENDER && g_log[6].self == Self() && g_log[6].value == 16.0);
    CHECK(g_ForceRenderFramesPerSec_013B8630 == 16 && Get<unsigned char>(0x161E2) == 1);
    CHECK(Get<unsigned long>(0x16138) == 20 && Get<long>(0x161E4) == 4 && g_RenderFrame_013B7D6C == 1234);

    // 5. Forced render above ConstantFPS is passed through.
    Reset();
    g_ForceRenderFramesPerSec_013B8630 = 40;
    Probe::Call(Self());
    CHECK(g_count == 7 && g_log[6].kind == EV_FORCE_RENDER && g_log[6].value == 40.0);
    CHECK(g_ForceRenderFramesPerSec_013B8630 == 40 && Get<unsigned char>(0x161E2) == 1);

    // 6. Normal frame, no bullet time, client free to render.
    Reset();
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 17);
    {
        const Event* e = g_log;
        CHECK(e[5].kind == EV_GET_FRAME);
        CHECK(e[6].kind == EV_NEXT_PRESENT && e[6].self == Self());
        CHECK(e[7].kind == EV_LIMIT && e[7].value == 0.5 && e[7].lo == 0.0 && e[7].hi == 1.0);
        CHECK(e[8].kind == EV_PREDICTED && e[8].self == Self());
        CHECK(e[9].kind == EV_BT_ACTIVE && e[9].self == Bullet());
        CHECK(e[10].kind == EV_RENDER_UPDATE && e[10].self == World() && e[10].arg == info);
        const CInterpolationInfo& in = e[10].info;
        CHECK(SameSet(in.Current, 0.125f, 0.5f) && SameSet(in.Last, 100.0f, 101.0f));
        CHECK(SameSet(in.PausedCurrent, 0.125f, 0.5f) && SameSet(in.PausedLast, 104.0f, 105.0f));
        CHECK(SameSet(in.BulletTimeCurrent, 0.125f, 0.5f) && SameSet(in.BulletTimeLast, 0.0f, 0.0f));
        CHECK(in.WFServerCurrentTime == 51);
        CHECK(memcmp(g_component + 0x70, &in, sizeof(in)) == 0);
        CHECK(e[11].kind == EV_FREE_TO_RENDER && e[11].self == Client());
        CHECK(e[12].kind == EV_FREE_TO_RENDER && e[12].self == Client());
        CHECK(RenderTail(13, info));
        CHECK(memcmp(&e[15].info, &in, sizeof(in)) == 0);
        CHECK(g_RenderFrame_013B7D6C == 1234 && Get<unsigned char>(0x161E3) == 0x5A &&
              Get<unsigned long>(0x16134) == 0xDEADBEEF && Get<double>(0x60) == 77.0);
    }

    // 7. Interpolate above 1 clamps; WorldUpdateTurn false is forwarded.
    Reset();
    Put<long>(0x48, 50);
    Put<unsigned char>(0x161B8, 0);
    Probe::Call(Self());
    CHECK(g_count == 17 && g_log[7].value == 1.5);
    CHECK(SameSet(g_log[10].info.Current, 0.125f, 1.0f) && g_log[10].info.WFServerCurrentTime == 50);
    CHECK(g_log[15].kind == EV_RENDER && g_log[15].flag1 == false && g_log[15].flag2 == true);

    // 8. Bullet time converts the current and last interpolates.
    Reset();
    g_bulletTime = true;
    Probe::Call(Self());
    CHECK(g_count == 19);
    CHECK(g_log[10].kind == EV_BT_CONVERT && g_log[10].self == Bullet() && g_log[10].value == 0.5);
    CHECK(g_log[11].kind == EV_BT_CONVERT && g_log[11].self == Bullet() && g_log[11].value == 101.0);
    {
        const CInterpolationInfo& in = g_log[12].info;
        CHECK(g_log[12].kind == EV_RENDER_UPDATE);
        CHECK(SameSet(in.BulletTimeCurrent, 0.125f, 2.0f) && SameSet(in.BulletTimeLast, 0.0f, 203.0f));
        CHECK(SameSet(in.Current, 0.125f, 0.5f) && SameSet(in.PausedCurrent, 0.125f, 0.5f));
    }

    // 9. Client not free to render: counted, not rendered.
    Reset();
    g_free[0] = g_free[1] = false;
    Probe::Call(Self());
    CHECK(g_count == 13 && g_log[11].kind == EV_FREE_TO_RENDER && g_log[12].kind == EV_FREE_TO_RENDER);
    CHECK(Get<long>(0x161E4) == 5 && Get<unsigned char>(0x161E2) == 0 &&
          Get<unsigned long>(0x16138) == 20 && Get<double>(0x68) == -1.0);
    CHECK(SameSet(*reinterpret_cast<const CInterpolationInfoSet*>(g_component + 0x70), 0.125f, 0.5f));

    // 10. AlwaysUpdateGameFrame renders anyway, resetting the counter.
    Reset();
    g_free[0] = g_free[1] = false;
    CMainGameComponent::AlwaysUpdateGameFrame = true;
    Probe::Call(Self());
    CHECK(g_count == 16 && g_log[11].kind == EV_FREE_TO_RENDER && RenderTail(12, g_log[0].self));

    // 11. Free on the first query only: counted once, then rendered.
    Reset();
    g_free[0] = false; g_free[1] = true;
    Probe::Call(Self());
    CHECK(g_count == 17 && RenderTail(13, g_log[0].self));

    // 12. Paused world: interpolation from the last frame, paused set recomputed.
    Reset();
    g_worldStorage[0xA4] = 1;
    g_bulletTime = true;
    Probe::Call(Self());
    CHECK(Prefix(&info) && g_count == 16);
    CHECK(g_log[6].kind == EV_NEXT_PRESENT && g_log[7].kind == EV_LIMIT && g_log[7].value == 0.5 &&
          g_log[8].kind == EV_PREDICTED && g_log[9].kind == EV_RENDER_UPDATE);
    {
        const CInterpolationInfo& in = g_log[9].info;
        CHECK(SameSet(in.Current, 100.0f, 101.0f) && SameSet(in.Last, 102.0f, 103.0f));
        CHECK(SameSet(in.PausedCurrent, 0.125f, 0.5f) && SameSet(in.PausedLast, 104.0f, 105.0f));
        CHECK(SameSet(in.BulletTimeCurrent, 108.0f, 109.0f) && SameSet(in.BulletTimeLast, 0.0f, 0.0f));
        CHECK(in.WFServerCurrentTime == 51 && memcmp(g_component + 0x70, &in, sizeof(in)) == 0);
    }
    CHECK(RenderTail(12, info));

    if (!ok)
    {
        printf("FABLETLC_MAINGAME_RENDER_FAIL\n");
        return 1;
    }
    printf("FABLETLC_MAINGAME_RENDER_PASS\n");
    return 0;
}