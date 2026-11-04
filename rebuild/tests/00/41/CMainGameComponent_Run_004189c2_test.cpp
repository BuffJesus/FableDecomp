#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_definition_manager.h"
#include "fable_user_profile.h"
#include "fable_memory_allocator.h"
#include "fable_world_map.h"

// Fixture for CMainGameComponent::Run 0x004189C2. Every callee is a recording
// stand-in; the log is (kind, receiver, integer argument, double argument).
namespace
{
    enum Kind
    {
        EV_WAIT_FOR_SYNC = 1,
        EV_GFGETTIME,
        EV_GAME_TIME,
        EV_BEGIN_LOADING,
        EV_BEGIN_SAVING,
        EV_PRESENTATION,
        EV_PEEK_WORLD_MAP,
        EV_LOADED_REGION,
        EV_GET_REGION,
        EV_DEF_RELEASED,
        EV_PROFILE_GET,
        EV_PROFILE_APPLY,
        EV_START_SCENE,
        EV_CONSOLIDATE,
        EV_CLEAR_STATS,
        EV_DIRTY_DISC,
        EV_SYSTEM_MANAGER,
        EV_SYSTEM_UPDATE,
        EV_PROFILER_UPDATE,
        EV_FORCED_TICK,
        EV_NEW_FRAME,
        EV_UPDATE_REGULAR,
        EV_AVERAGE,
        EV_LOG_MEMORY,
        EV_DEFS_GET,
        EV_SCRAMBLE,
        EV_SHUTDOWN
    };

    struct Event
    {
        int kind;
        const void* ptr;
        long arg;
        double value;
    };

    Event g_log[128];
    int g_count;

    void Log(int kind, const void* ptr, long arg = 0, double value = 0.0)
    {
        if (g_count < 128)
        {
            g_log[g_count].kind = kind;
            g_log[g_count].ptr = ptr;
            g_log[g_count].arg = arg;
            g_log[g_count].value = value;
        }
        ++g_count;
    }

    int g_cursor;
    bool Next(int kind, const void* ptr, long arg = 0, double value = 0.0)
    {
        const int i = g_cursor++;
        return i < g_count && i < 128 && g_log[i].kind == kind && g_log[i].ptr == ptr &&
            g_log[i].arg == arg && g_log[i].value == value;
    }

    unsigned long Dword(const unsigned char* p, unsigned long offset)
    {
        unsigned long value;
        memcpy(&value, p + offset, sizeof(value));
        return value;
    }

    double Double(const unsigned char* p, unsigned long offset)
    {
        double value;
        memcpy(&value, p + offset, sizeof(value));
        return value;
    }

    void PutDword(unsigned char* p, unsigned long offset, const void* value)
    {
        memcpy(p + offset, &value, sizeof(value));
    }

    // Retail CMainGameComponent offsets.
    const unsigned long kQuit = 0x08;
    const unsigned long kRunning = 0x09;
    const unsigned long kPWorld = 0x24;
    const unsigned long kPDisplayEngine = 0x28;
    const unsigned long kForceUpdateTick = 0x34;
    const unsigned long kCurrentFrameStartGameTime = 0x58;
    const unsigned long kGameStartTime = 0x60;
    const unsigned long kClient = 0x13AB8;
    const unsigned long kLastRenderFrameStartTime = 0x161A8;
    const unsigned long kRoughFPSCounter = 0x161BC;
    const unsigned long kNextComponentToRun = 0x161C4;

    unsigned char* g_component;

    // Scripted clocks: GFGetTime returns 100, 101, ...; GetCurrentGameTime
    // returns 10, 20, 30, ...
    double g_realTime;
    double g_gameTime;

    // Scripted CSystemManager::Update results and profile-manager presence.
    int g_updateResults[8];
    bool g_profilePresent[8];
    int g_updateCount;
    unsigned char g_systemManager[sizeof(CSystemManager)];
    unsigned char g_profileManager[sizeof(CProfileManager)];
    bool g_setForceTickOnRegular;
    int g_armStep;

    unsigned char g_userProfile[sizeof(CUserProfileManager)];
    unsigned char g_defs[sizeof(CGameDefinitionManager)];
    unsigned char g_memoryAllocator[16];
    unsigned char g_displayManager[sizeof(CDisplayManager)];

    // The world, its map and region, faked as vtable-dispatched objects.
    struct Recorder
    {
        const CWorldMap& PeekWorldMap() const;
        void StartNewScene(unsigned long flags, float time);
    };

    unsigned char g_worldMap[16];
    void* g_worldVtable[16];
    void* g_engineVtable[64];
    struct FakeObject
    {
        void** vptr;
        int payload[16];
    };
    FakeObject g_world;
    FakeObject g_engine;
    unsigned char g_displayEngine[0x40];
    unsigned char g_region[sizeof(CRegion)];
    long g_loadedRegion = 7;

    class TestDef : public CRegionDef
    {
    public:
        virtual void DeleteCIVCountedPointeeBase() const { Log(EV_DEF_RELEASED, this); }
    };
    TestDef g_def;

    const CWorldMap& Recorder::PeekWorldMap() const
    {
        Log(EV_PEEK_WORLD_MAP, this);
        return *reinterpret_cast<const CWorldMap*>(g_worldMap);
    }

    void Recorder::StartNewScene(unsigned long flags, float time)
    {
        Log(EV_START_SCENE, this, (long)flags, time);
    }
}

// Link stand-ins / recorders for the real callees.
// The def base is CResource (retail); its out-of-line pieces.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
CResource::CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) {}
void CResourceList::RemoveResource(CResource&) {}
void CNetworkClient::WaitForSync() { Log(EV_WAIT_FOR_SYNC, this); }
double FABLE_FASTCALL GFGetTime()
{
    g_realTime += 1.0;
    Log(EV_GFGETTIME, 0, 0, g_realTime);
    return g_realTime;
}
double CMainGameComponent::GetCurrentGameTime() const
{
    g_gameTime += 10.0;
    Log(EV_GAME_TIME, this, 0, g_gameTime);
    return g_gameTime;
}
void CMainGameComponent::BeginInputLoading(const CWideString& name) { Log(EV_BEGIN_LOADING, this, (long)&name); }
void CMainGameComponent::BeginInputSaving(const CWideString& name) { Log(EV_BEGIN_SAVING, this, (long)&name); }
void CDisplayManager::ChangePresentationInterval(bool enable, long interval, bool apply)
{
    Log(EV_PRESENTATION, this, (enable ? 0x100 : 0) | (interval << 4) | (apply ? 1 : 0));
}
long CWorldMap::GetLoadedRegion() const
{
    Log(EV_LOADED_REGION, this);
    return g_loadedRegion;
}
const CRegion& CWorldMap::GetRegion(long index) const
{
    Log(EV_GET_REGION, this, index);
    return *reinterpret_cast<const CRegion*>(g_region);
}
CUserProfileManager& FABLE_FASTCALL CUserProfileManager::Get()
{
    Log(EV_PROFILE_GET, 0);
    return *reinterpret_cast<CUserProfileManager*>(g_userProfile);
}
void CUserProfileManager::Apply() { Log(EV_PROFILE_APPLY, this); }
CMemoryAllocator* g_FableMemoryAllocator_013CAA90 =
    reinterpret_cast<CMemoryAllocator*>(g_memoryAllocator);
void CMemoryAllocator::ConsolidateFreeAreas() { Log(EV_CONSOLIDATE, this); }
void CMemoryAllocator::ClearPerformanceStats() { Log(EV_CLEAR_STATS, this); }
void FABLE_FASTCALL CThreadedFileManager::ProcessDirtyDiscError() { Log(EV_DIRTY_DISC, 0); }
CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Log(EV_SYSTEM_MANAGER, 0);
    return reinterpret_cast<CSystemManager*>(g_systemManager);
}
ESystemReturn CSystemManager::Update()
{
    const int step = g_updateCount++;
    g_setForceTickOnRegular = (step == g_armStep);
    ProfileManager.Data = g_profilePresent[step] ?
        reinterpret_cast<CProfileManager*>(g_profileManager) : 0;
    Log(EV_SYSTEM_UPDATE, this, g_updateResults[step]);
    return (ESystemReturn)g_updateResults[step];
}
void CProfileManager::Update() { Log(EV_PROFILER_UPDATE, this); }
void CMainGameComponent::UpdateForcedTickSpeed() { Log(EV_FORCED_TICK, this); }
void CRoughFrameCounter::NewFrame() { Log(EV_NEW_FRAME, this); }
void CMainGameComponent::UpdateRegular()
{
    Log(EV_UPDATE_REGULAR, this);
    if (g_setForceTickOnRegular)
        g_component[kForceUpdateTick] = 1;
}
void CMainGameComponent::UpdateAverageFrameDuration(double duration) { Log(EV_AVERAGE, this, 0, duration); }
void FABLE_FASTCALL GFLogMemoryUse() { Log(EV_LOG_MEMORY, 0); }
CGameDefinitionManager& FABLE_FASTCALL CGameDefinitionManager::Get()
{
    Log(EV_DEFS_GET, 0);
    return *reinterpret_cast<CGameDefinitionManager*>(g_defs);
}
void CDefinitionManager::ScrambleDefs(bool scramble) { Log(EV_SCRAMBLE, this, scramble ? 1 : 0); }
void CMainGameComponent::Shutdown()
{
    // Run has already published the next component and cleared Running.
    Log(EV_SHUTDOWN, this, g_component[kRunning]);
}

bool g_FableLoadInputEvents_013B85F6;
bool g_FableSaveInputEvents_013B85F5;
bool g_FablePresentationIntervalOverride_013B8628;
CWideString g_FableInputEventsFileName_013B865C;
CDisplayManager* g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(g_displayManager);
CGameComponent* g_FableRetiredGameComponent_013B7D58;

CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}

namespace
{
    void Reset()
    {
        g_count = 0;
        g_cursor = 0;
        g_realTime = 100.0;
        g_gameTime = 0.0;
        g_updateCount = 0;
        g_setForceTickOnRegular = false;
        g_armStep = -1;
        g_FableRetiredGameComponent_013B7D58 = 0;
    }

    unsigned char* NewComponent()
    {
        unsigned char* p = static_cast<unsigned char*>(malloc(sizeof(CMainGameComponent)));
        memset(p, 0xA5, sizeof(CMainGameComponent));
        p[kQuit] = 1;            // Run clears it before the loop
        p[kRunning] = 0;
        p[kForceUpdateTick] = 0;
        PutDword(p, kPWorld, &g_world);
        PutDword(p, kPDisplayEngine, g_displayEngine);
        g_component = p;
        return p;
    }

    // The fixed frame tail: frame duration, memory log, def scramble.
    bool FrameTail(unsigned char* p, double duration)
    {
        return Next(EV_GAME_TIME, p, 0, g_log[g_cursor].value) &&
            Next(EV_AVERAGE, p, 0, duration) &&
            Next(EV_LOG_MEMORY, 0) &&
            Next(EV_DEFS_GET, 0) &&
            Next(EV_SCRAMBLE, g_defs, 1);
    }

    bool Prologue(unsigned char* p)
    {
        return Next(EV_WAIT_FOR_SYNC, p + kClient) &&
            Next(EV_GFGETTIME, 0, 0, 101.0) &&
            Next(EV_GAME_TIME, p, 0, 10.0);
    }

    bool Epilogue(unsigned char* p)
    {
        return Next(EV_PROFILE_GET, 0) &&
            Next(EV_PROFILE_APPLY, g_userProfile) &&
            Next(EV_START_SCENE, &g_engine, 12, 20.0) &&
            Next(EV_CONSOLIDATE, g_memoryAllocator) &&
            Next(EV_CLEAR_STATS, g_memoryAllocator);
    }
}

int main()
{
    // Fake vtables: CWorld slot 13 is PeekWorldMap, CIEngine slot 55 StartNewScene.
    union
    {
        const CWorldMap& (Recorder::*method)() const;
        void* address;
    } peek;
    peek.method = &Recorder::PeekWorldMap;
    union
    {
        void (Recorder::*method)(unsigned long, float);
        void* address;
    } start;
    start.method = &Recorder::StartNewScene;
    g_worldVtable[13] = peek.address;
    g_engineVtable[55] = start.address;
    g_world.vptr = g_worldVtable;
    g_engine.vptr = g_engineVtable;
    PutDword(g_displayEngine, 0x2C, &g_engine);    // CDisplayEngine::Engine3D

    // 1. Replay input, presentation override, five frames:
    //    OK+profiler (regular), SYSTEM_LOCK (no update), OK without profiler
    //    (regular; the stand-in turns ForceUpdateTick on), OK (forced), QUIT.
    Reset();
    g_FableLoadInputEvents_013B85F6 = true;
    g_FableSaveInputEvents_013B85F5 = true;
    g_FablePresentationIntervalOverride_013B8628 = true;
    const int results1[5] = { ESR_OK, ESR_SYSTEM_LOCK, ESR_OK, ESR_OK, ESR_QUIT };
    const bool profile1[5] = { true, true, false, true, true };
    for (int i = 0; i < 5; ++i)
    {
        g_updateResults[i] = results1[i];
        g_profilePresent[i] = profile1[i];
    }
    unsigned char* a = NewComponent();
    // Frame 3's UpdateRegular turns ForceUpdateTick on, so frame 4 takes the
    // forced path (Run re-reads the flag every frame).
    g_armStep = 2;
    CGameComponent* next = reinterpret_cast<CGameComponent*>(0x1111);
    CGameComponent* const nextComponent = reinterpret_cast<CGameComponent*>(0x2222);
    PutDword(a, kNextComponentToRun, nextComponent);
    // Run.
    const bool result1 = reinterpret_cast<CMainGameComponent*>(a)->CMainGameComponent::Run(&next);
    g_cursor = 0;
    bool ok1 = Prologue(a) &&
        Next(EV_BEGIN_LOADING, a, (long)&g_FableInputEventsFileName_013B865C) &&
        Next(EV_PRESENTATION, g_displayManager, 0x100 | (1 << 4) | 1) &&
        Epilogue(a);
    // Frame 1: OK, profiler present, regular update.
    ok1 = ok1 && Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, a, 0, 20.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_OK) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_MANAGER, 0) &&
        Next(EV_PROFILER_UPDATE, g_profileManager) &&
        Next(EV_NEW_FRAME, a + kRoughFPSCounter) && Next(EV_UPDATE_REGULAR, a) &&
        FrameTail(a, 30.0 - 20.0);
    // Frame 2: SYSTEM_LOCK, nothing updated.
    ok1 = ok1 && Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, a, 0, 40.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_SYSTEM_LOCK) &&
        FrameTail(a, 50.0 - 40.0);
    // Frame 3: OK, no profiler, regular.
    ok1 = ok1 && Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, a, 0, 60.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_OK) &&
        Next(EV_SYSTEM_MANAGER, 0) &&
        Next(EV_NEW_FRAME, a + kRoughFPSCounter) && Next(EV_UPDATE_REGULAR, a) &&
        FrameTail(a, 70.0 - 60.0);
    // Frame 4: OK, profiler, ForceUpdateTick now on: forced update only.
    ok1 = ok1 && Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, a, 0, 80.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_OK) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_MANAGER, 0) &&
        Next(EV_PROFILER_UPDATE, g_profileManager) &&
        Next(EV_FORCED_TICK, a) &&
        FrameTail(a, 90.0 - 80.0);
    // Frame 5: QUIT sets Quit; the frame tail still runs; then the epilogue.
    ok1 = ok1 && Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, a, 0, 100.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_QUIT) &&
        FrameTail(a, 110.0 - 100.0) &&
        Next(EV_SHUTDOWN, a, 0) && g_cursor == g_count;
    ok1 = ok1 && result1 && next == nextComponent &&
        g_FableRetiredGameComponent_013B7D58 == reinterpret_cast<CGameComponent*>(a) &&
        a[kQuit] == 1 && a[kRunning] == 0 &&
        Double(a, kGameStartTime) == 101.0 &&
        Double(a, kLastRenderFrameStartTime) == 10.0 &&
        Double(a, kCurrentFrameStartGameTime) == 100.0 &&
        Dword(a, kPWorld) == (unsigned long)&g_world;
    free(a);

    // 2. Record input, no override (region def path, def stays referenced),
    //    ForceUpdateTick on: forced update path; next component null -> false.
    Reset();
    g_FableLoadInputEvents_013B85F6 = false;
    g_FableSaveInputEvents_013B85F5 = true;
    g_FablePresentationIntervalOverride_013B8628 = false;
    g_updateResults[0] = ESR_OK;
    g_profilePresent[0] = false;
    g_updateResults[1] = ESR_QUIT;
    g_profilePresent[1] = false;
    g_def.IVRefCount = 1;
    PutDword(g_region, 0x24, &g_def);
    unsigned char* b = NewComponent();
    b[kForceUpdateTick] = 1;
    PutDword(b, kNextComponentToRun, 0);
    next = reinterpret_cast<CGameComponent*>(0x1111);
    const bool result2 = reinterpret_cast<CMainGameComponent*>(b)->CMainGameComponent::Run(&next);
    g_cursor = 0;
    bool ok2 = Prologue(b) &&
        Next(EV_BEGIN_SAVING, b, (long)&g_FableInputEventsFileName_013B865C) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) &&
        Next(EV_LOADED_REGION, g_worldMap) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) &&
        Next(EV_GET_REGION, g_worldMap, 7) &&
        Epilogue(b) &&
        Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, b, 0, 20.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_OK) &&
        Next(EV_SYSTEM_MANAGER, 0) &&
        Next(EV_FORCED_TICK, b) &&
        FrameTail(b, 10.0) &&
        Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, b, 0, 40.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_QUIT) &&
        FrameTail(b, 10.0) &&
        Next(EV_SHUTDOWN, b, 0) && g_cursor == g_count;
    ok2 = ok2 && !result2 && next == 0 && g_def.IVRefCount == 1 &&
        g_FableRetiredGameComponent_013B7D58 == reinterpret_cast<CGameComponent*>(b) &&
        b[kQuit] == 1 && b[kRunning] == 0;
    free(b);

    // 3. Neither input flag, no override, the def's last reference is the
    //    temporary copy's: it is released (slot 1). A null def is skipped.
    Reset();
    g_FableLoadInputEvents_013B85F6 = false;
    g_FableSaveInputEvents_013B85F5 = false;
    g_updateResults[0] = ESR_QUIT;
    g_profilePresent[0] = true;
    g_def.IVRefCount = 0;
    unsigned char* c = NewComponent();
    PutDword(c, kNextComponentToRun, 0);
    reinterpret_cast<CMainGameComponent*>(c)->CMainGameComponent::Run(&next);
    g_cursor = 0;
    bool ok3 = Prologue(c) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) && Next(EV_LOADED_REGION, g_worldMap) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) && Next(EV_GET_REGION, g_worldMap, 7) &&
        Next(EV_DEF_RELEASED, &g_def) &&
        Epilogue(c) &&
        Next(EV_DIRTY_DISC, 0) && Next(EV_GAME_TIME, c, 0, 20.0) &&
        Next(EV_SYSTEM_MANAGER, 0) && Next(EV_SYSTEM_UPDATE, g_systemManager, ESR_QUIT) &&
        FrameTail(c, 10.0) && Next(EV_SHUTDOWN, c, 0) && g_cursor == g_count &&
        g_def.IVRefCount == 0;

    Reset();
    g_updateResults[0] = ESR_QUIT;
    PutDword(g_region, 0x24, 0);
    reinterpret_cast<CMainGameComponent*>(c)->CMainGameComponent::Run(&next);
    g_cursor = 0;
    ok3 = ok3 && Prologue(c) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) && Next(EV_LOADED_REGION, g_worldMap) &&
        Next(EV_PEEK_WORLD_MAP, &g_world) && Next(EV_GET_REGION, g_worldMap, 7) &&
        Epilogue(c);
    free(c);

    if (!ok1 || !ok2 || !ok3)
    {
        printf("ok1=%d ok2=%d ok3=%d cursor=%d count=%d\n", ok1, ok2, ok3, g_cursor, g_count);
        for (int i = 0; i < g_count && i < 128; ++i)
            printf("  %d kind=%d ptr=%p arg=%ld value=%g\n", i, g_log[i].kind, g_log[i].ptr,
                g_log[i].arg, g_log[i].value);
        puts("FABLETLC_MAIN_GAME_COMPONENT_RUN FAIL");
        return 1;
    }
    puts("FABLETLC_MAIN_GAME_COMPONENT_RUN PASS");
    return 0;
}