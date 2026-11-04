#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "fable_world.h"
#include "engine/CDisplayEngine.h"

#define CHECK(x) do { if (!(x)) { printf("check failed at line %d\n", __LINE__); ok = false; } } while (0)

// Fixture for CMainGameComponent::ForceRender(long) 0x004164E0. Every callee
// is a recording stand-in; the log keeps (kind, receiver, argument) triples.
namespace
{
    enum Kind { EV_CTOR = 1, EV_RENDER_UPDATE, EV_TIME, EV_RENDER };

    struct Event
    {
        int kind;
        const void* self;
        const void* arg;
        bool flag1;
        bool flag2;
        CInterpolationInfo info;
        double lastRenderStart;
    };

    Event g_log[64];
    int g_count;
    double g_times[64];
    int g_timeIndex;
    unsigned char* g_component;

    Event& Log(int kind, const void* self, const void* arg)
    {
        static Event overflow;
        if (g_count >= 64) { ++g_count; return overflow; }
        Event& e = g_log[g_count++];
        memset(&e, 0, sizeof(e));
        e.kind = kind; e.self = self; e.arg = arg;
        return e;
    }

    struct Probe : CMainGameComponent
    {
        static void Call(CMainGameComponent* component, long fps)
        {
            static_cast<Probe*>(component)->CMainGameComponent::ForceRender(fps);
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
}

CInterpolationInfo::CInterpolationInfo() : WFServerCurrentTime(0)
{
    Log(EV_CTOR, this, 0);
}

void CWorld::RenderUpdate(const CInterpolationInfo* info)
{
    Event& e = Log(EV_RENDER_UPDATE, this, info);
    e.info = *info;
}

double CMainGameComponent::GetCurrentGameTime() const
{
    Event& e = Log(EV_TIME, this, 0);
    (void)e;
    return g_times[g_timeIndex++];
}

void CDisplayEngine::Render(const CInterpolationInfo* info, bool worldUpdateTurn, bool present)
{
    Event& e = Log(EV_RENDER, this, info);
    e.flag1 = worldUpdateTurn;
    e.flag2 = present;
    e.info = *info;
    e.lastRenderStart = Get<double>(0x161A8);
}

static void Reset(unsigned char* component, CWorld* world, CDisplayEngine* engine)
{
    memset(component, 0, sizeof(CMainGameComponent));
    Put<void*>(0x24, world);
    Put<void*>(0x28, engine);
    Put<long>(0x48, 7);                // CurrentServerFrame
    Put<long>(0x50, 3);                // LastGameTurnForceRendered
    Put<double>(0x68, -1.0);           // LastFrameRenderDuration
    // LastInterpolationInfo +0x70: distinct sentinels in every set.
    for (int i = 0; i < 12; ++i)
        Put<float>(0x70 + 4 * i, 100.0f + i);
    Put<long>(0xA0, 55);
    Put<unsigned long>(0x16138, 20);   // NoRenderFramesSinceLastGameUpdate
    Put<unsigned char>(0x161B8, 1);    // WorldUpdateTurn
    Put<unsigned char>(0x161B9, 0xCC); // neighbour
    Put<unsigned long>(0x16134, 0xDEADBEEF);  // neighbour before NoRenderFrames
    g_count = 0;
    g_timeIndex = 0;
    for (int t = 0; t < 64; ++t)
        g_times[t] = 10.0 + t * 0.5;
}

int main()
{
    unsigned char* component = static_cast<unsigned char*>(malloc(sizeof(CMainGameComponent)));
    g_component = component;
    unsigned char worldStorage[sizeof(CWorld)];
    unsigned char engineStorage[sizeof(CDisplayEngine)];
    CWorld* world = reinterpret_cast<CWorld*>(worldStorage);
    CDisplayEngine* engine = reinterpret_cast<CDisplayEngine*>(engineStorage);
    CMainGameComponent* self = reinterpret_cast<CMainGameComponent*>(component);
    bool ok = true;

    // 60 fps -> 4 forced frames, each 1/60 s further on.
    Reset(component, world, engine);
    Probe::Call(self, 60);
    const float frameTime = 1.0f / 60;
    CHECK(g_count == 4 * 6);
    for (int i = 0; i < 4; ++i)
    {
        const Event* e = &g_log[i * 6];
        const float gt = (float)(i + 1) * frameTime;
        const float wf = (float)i / 4.0f;
        CHECK(e[0].kind == EV_CTOR);
        const void* info = e[0].self;
        CHECK(e[1].kind == EV_RENDER_UPDATE && e[1].self == world && e[1].arg == info);
        CHECK(e[2].kind == EV_TIME && e[2].self == self);
        CHECK(e[3].kind == EV_TIME && e[3].self == self);
        CHECK(e[4].kind == EV_RENDER && e[4].self == engine && e[4].arg == info &&
            e[4].flag1 == true && e[4].flag2 == true);
        CHECK(e[4].lastRenderStart == g_times[i * 3 + 1]);
        CHECK(e[5].kind == EV_TIME && e[5].self == self);
        const CInterpolationInfo& in = e[1].info;
        CHECK(SameSet(in.Current, gt, wf) && SameSet(in.PausedCurrent, gt, wf));
        if (i == 0)
            CHECK(SameSet(in.Last, 100.0f, 101.0f) && SameSet(in.PausedLast, 104.0f, 105.0f));
        else
        {
            const float pgt = (float)i * frameTime, pwf = (float)(i - 1) / 4.0f;
            CHECK(SameSet(in.Last, pgt, pwf) && SameSet(in.PausedLast, pgt, pwf));
        }
        CHECK(SameSet(in.BulletTimeCurrent, 0.0f, 0.0f) && SameSet(in.BulletTimeLast, 0.0f, 0.0f));
        CHECK(in.WFServerCurrentTime == 7);
        CHECK(memcmp(&e[4].info, &in, sizeof(in)) == 0);
    }
    CHECK(memcmp(component + 0x70, &g_log[19].info, sizeof(CInterpolationInfo)) == 0);
    CHECK(Get<long>(0x50) == 7);
    CHECK(Get<unsigned long>(0x16138) == 24);
    CHECK(Get<double>(0x161A8) == g_times[10]);
    CHECK(Get<double>(0x68) == g_times[11] - g_times[9]);
    CHECK(Get<unsigned char>(0x161B9) == 0xCC && Get<unsigned long>(0x16134) == 0xDEADBEEF);
    CHECK(Get<long>(0x48) == 7);

    // WorldUpdateTurn is forwarded as-is.
    Reset(component, world, engine);
    Put<unsigned char>(0x161B8, 0);
    Probe::Call(self, 15);
    CHECK(g_count == 6 && g_log[4].kind == EV_RENDER && g_log[4].flag1 == false &&
        g_log[4].flag2 == true);
    CHECK(SameSet(g_log[1].info.Current, 1.0f / 15, 0.0f));

    // Already force-rendered this server frame: nothing happens.
    Reset(component, world, engine);
    Put<long>(0x50, 7);
    Probe::Call(self, 60);
    CHECK(g_count == 0 && Get<double>(0x68) == -1.0 && Get<unsigned long>(0x16138) == 20);

    // Fewer than DEFAULT_SERVER_FPS: zero frames, state untouched.
    Reset(component, world, engine);
    Probe::Call(self, 14);
    CHECK(g_count == 0 && Get<long>(0x50) == 3);

    if (!ok)
    {
        printf("FABLETLC_MAINGAME_FORCE_RENDER_FAIL\n");
        return 1;
    }
    printf("FABLETLC_MAINGAME_FORCE_RENDER_PASS\n");
    return 0;
}