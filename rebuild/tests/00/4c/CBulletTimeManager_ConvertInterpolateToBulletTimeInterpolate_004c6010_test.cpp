// Behaviour: result = ((GetFrame()-1) % (long)m) / m + interpolate / m, with the
// multiplier m queried from `this`; checked over several frames/multipliers.
#include "engine/CBulletTimeManager.h"
#include "fable_world.h"
#include <stdio.h>
#include <math.h>

static float g_multiplier;
static long g_frame;
static const void* g_this;
static int g_multCalls;
float CBulletTimeManager::GetBulletTimeSlowdownMultiplier() const { ++g_multCalls; g_this = this; return g_multiplier; }
long CWorld::GetFrame() { return g_frame; }

static int Check(float m, long frame, float interp, float expect)
{
    unsigned char storage[sizeof(CBulletTimeManager)];
    const CBulletTimeManager* btm = reinterpret_cast<const CBulletTimeManager*>(storage);
    g_multiplier = m; g_frame = frame; g_multCalls = 0; g_this = 0;
    float r = btm->ConvertInterpolateToBulletTimeInterpolate(interp);
    if (g_multCalls != 1 || g_this != btm) return 1;
    if (fabs(r - expect) > 1e-5f) { printf("  got %f want %f\n", r, expect); return 2; }
    return 0;
}

int main()
{
    int r;
    if ((r = Check(1.0f, 10, 0.5f, 0.5f)) != 0) { printf("FAIL m=1 %d\n", r); return 1; }
    if ((r = Check(4.0f, 1, 0.0f, 0.0f)) != 0) { printf("FAIL cycle start %d\n", r); return 2; }
    if ((r = Check(4.0f, 3, 0.5f, 0.625f)) != 0) { printf("FAIL mid cycle %d\n", r); return 3; }
    if ((r = Check(4.0f, 8, 1.0f, 1.0f)) != 0) { printf("FAIL cycle end %d\n", r); return 4; }
    if ((r = Check(2.5f, 6, 0.25f, 0.1f + 0.4f)) != 0) { printf("FAIL truncated multiplier %d\n", r); return 5; }
    printf("FABLETLC_BULLETTIME_CONVERT_INTERPOLATE PASS\n");
    return 0;
}