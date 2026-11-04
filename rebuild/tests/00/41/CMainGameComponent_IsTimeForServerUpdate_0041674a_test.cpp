// Behaviour fixture for CMainGameComponent::IsTimeForServerUpdate @ 0x0041674A.
// Running is component +0x09; GPInputManager's Loading flag +0x38;
// ConstantFPS is 30 here, so a game time t is t*30 world frames. Checks:
// not running -> false with nothing else read; the movie-recording replay
// case (render-only, loading, not recording) -> true without timing; the
// AlwaysUpdateGameFrame override -> true without timing; otherwise the next
// present's time is fetched once and it is time only when it lands more than
// one frame after `frame` (exactly one is not enough). Nothing is written.
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

CInputManager* g_FableInputManager_013B8388;
bool g_RenderOnlyWhenRecordingMovie_013B860C;
bool g_RecordMovie_013B8629;
bool CMainGameComponent::AlwaysUpdateGameFrame;
long CMainGameComponent::ConstantFPS = 30;

static unsigned char comp[0x161F0];
static unsigned char inputManager[0x84];
static int timeCalls;
static double nextPresent;
static bool ok;

double CMainGameComponent::GetGameTimeOfNextPresentCompletion() const
{
    ok = ok && static_cast<const void*>(this) == comp;
    ++timeCalls;
    return nextPresent;
}

static bool Run(const char* name, bool running, bool renderOnly, bool loading, bool recording,
                bool always, double present, long frame, bool expect, int expectTime)
{
    g_FableInputManager_013B8388 = reinterpret_cast<CInputManager*>(inputManager);
    memset(comp, 0x00, sizeof(comp));
    memset(inputManager, 0, sizeof(inputManager));
    comp[0x09] = running ? 1 : 0;
    inputManager[0x38] = loading ? 1 : 0;
    g_RenderOnlyWhenRecordingMovie_013B860C = renderOnly;
    g_RecordMovie_013B8629 = recording;
    CMainGameComponent::AlwaysUpdateGameFrame = always;
    nextPresent = present;
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    timeCalls = 0;
    ok = true;
    bool result = reinterpret_cast<CMainGameComponent*>(comp)->IsTimeForServerUpdate(frame);
    ok = ok && result == expect && timeCalls == expectTime && memcmp(before, comp, sizeof(comp)) == 0;
    if (!ok) printf("case %s failed (result %d, time calls %d)\n", name, result, timeCalls);
    return ok;
}

int main()
{
    bool pass = true;
    //            name              run    ronly  load   rec    always present frame expect time
    pass = Run("not running",     false, true,  true,  false, true,  10.0,  0,    false, 0) && pass;
    pass = Run("movie replay",    true,  true,  true,  false, false, 0.0,   1000, true,  0) && pass;
    pass = Run("recording",       true,  true,  true,  true,  false, 0.0,   1000, false, 1) && pass;
    pass = Run("not loading",     true,  true,  false, false, false, 0.0,   1000, false, 1) && pass;
    pass = Run("always update",   true,  false, false, false, true,  0.0,   1000, true,  0) && pass;
    pass = Run("two frames ahead",true,  false, false, false, false, 1.0,   28,   true,  1) && pass;
    pass = Run("one frame ahead", true,  false, false, false, false, 1.0,   29,   false, 1) && pass;
    pass = Run("1.5 frames ahead",true,  false, false, false, false, 1.05,  30,   true,  1) && pass;
    pass = Run("behind",          true,  false, false, false, false, 1.0,   40,   false, 1) && pass;
    printf(pass ? "PASS_0041674a\n" : "FAIL_0041674a\n");
    return pass ? 0 : 1;
}
