#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_game.h"

#define CHECK(x) do { if (!(x)) { printf("check failed at line %d\n", __LINE__); ok = false; } } while (0)

// Fixture for CMainGameComponent::GetGameTimeOfNextPresentCompletion 0x004166E2.
// The display manager, its vsync timer and GFGetTime are recording stand-ins.
namespace
{
    bool* g_useOld;
    CVsyncCallbackTimerInfo g_info;
    double g_now;
    int g_getInfoCalls;
    int g_timeCalls;
    const void* g_getInfoThis;

    bool* RegisterPrivates(bool* useOld)
    {
        g_useOld = useOld;
        return useOld;
    }

    struct Probe : CMainGameComponent
    {
        static double Call(const CMainGameComponent* component)
        {
            return static_cast<const Probe*>(component)->
                CMainGameComponent::GetGameTimeOfNextPresentCompletion();
        }
    };

    void PutDouble(unsigned char* p, unsigned long offset, double value)
    {
        memcpy(p + offset, &value, sizeof(value));
    }
}

bool CMainGameComponent::UseOldInterpolationTiming =
    RegisterPrivates(&UseOldInterpolationTiming) != 0 && false;
CDisplayManager* g_FableDisplayManager_013B8390;

CVsyncCallbackTimerInfo CVsyncCallbackTimer::GetInfo()
{
    ++g_getInfoCalls;
    g_getInfoThis = this;
    return g_info;
}

double FABLE_FASTCALL GFGetTime()
{
    ++g_timeCalls;
    return g_now;
}

static bool RunCase(unsigned char* component, bool useOld, double now, double expected,
                    const void* timer)
{
    *g_useOld = useOld;
    g_now = now;
    g_getInfoCalls = 0;
    g_timeCalls = 0;
    g_getInfoThis = 0;
    const double result = Probe::Call(reinterpret_cast<CMainGameComponent*>(component));
    return result == expected && g_getInfoCalls == 1 && g_timeCalls == 1 &&
        g_getInfoThis == timer;
}

int main()
{
    unsigned char* component = static_cast<unsigned char*>(calloc(1, sizeof(CMainGameComponent)));
    unsigned char* manager = static_cast<unsigned char*>(calloc(1, sizeof(CDisplayManager)));
    unsigned char timerStorage[16];
    CVsyncCallbackTimer* timer = reinterpret_cast<CVsyncCallbackTimer*>(timerStorage);

    CDisplayManager* displayManager = reinterpret_cast<CDisplayManager*>(manager);
    new (&displayManager->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>(reinterpret_cast<CVsyncCallbackTimer*>(timer));
    displayManager->PresentationIntervalTime = 0.25f;
    g_FableDisplayManager_013B8390 = displayManager;

    PutDouble(component, 0x60, 1.5);         // GameStartTime
    PutDouble(component, 0x58, 99.0);        // CurrentFrameStartGameTime (must be ignored)

    memset(&g_info, 0, sizeof(g_info));
    g_info.CPUTimeStamp = 100.0;             // decoys
    g_info.CPUPrevTimeStamp = 200.0;
    g_info.GPUPrevTimeStamp = 300.0;
    g_info.CPUFrameCount = 10;
    g_info.GPUFrameCount = 8;
    g_info.GPUTimeStamp = 5.0;               // next present = 5 + (10-8+1)*0.25 = 5.75

    bool ok = true;
    CHECK(RunCase(component, false, 5.5, 5.75 - 1.5, timer));   // present still ahead
    CHECK(RunCase(component, false, 6.0, 6.0 - 1.5, timer));    // present already passed
    CHECK(RunCase(component, false, 5.75, 5.75 - 1.5, timer));  // equal
    CHECK(RunCase(component, true, 5.5, 5.5 - 1.5, timer));     // old timing: now
    CHECK(RunCase(component, true, 6.0, 6.0 - 1.5, timer));
    g_info.CPUFrameCount = 7;                                         // GPU ahead: 5 + 0*0.25
    CHECK(RunCase(component, false, 4.0, 5.0 - 1.5, timer));

    if (!ok)
    {
        printf("FABLETLC_MAINGAME_NEXT_PRESENT_FAIL\n");
        return 1;
    }
    printf("FABLETLC_MAINGAME_NEXT_PRESENT_PASS\n");
    return 0;
}