#include <float.h>
#include <stdio.h>

#include "fable_display_viewport.h"

static CDisplayManager* g_observedManager = 0;
static C2DBoxI g_observedViewport = {};
static fable_u32 g_calls = 0;

void CDisplayManager::SetIntegerViewportEndpoint(
    const C2DBoxI& viewport)
{
    g_observedManager = this;
    g_observedViewport = viewport;
    ++g_calls;
}

static bool CheckViewport(
    fable_i32 left,
    fable_i32 top,
    fable_i32 right,
    fable_i32 bottom)
{
    return g_observedViewport.left == left &&
           g_observedViewport.top == top &&
           g_observedViewport.right == right &&
           g_observedViewport.bottom == bottom;
}

int main()
{
    CDisplayManager display;

    _controlfp(_RC_NEAR, _MCW_RC);
    C2DBoxF nearest = {10.6f, -20.6f, 630.4f, 460.5f};
    display.SetViewport(nearest);
    if (g_observedManager != &display ||
        g_calls != 1 ||
        !CheckViewport(11, -21, 630, 460))
    {
        printf("FABLETLC_DISPLAY_FLOAT_VIEWPORT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    _controlfp(_RC_DOWN, _MCW_RC);
    C2DBoxF downward = {10.9f, -20.1f, 630.9f, -460.1f};
    display.SetViewport(downward);
    _controlfp(_RC_NEAR, _MCW_RC);
    if (g_calls != 2 ||
        !CheckViewport(10, -21, 630, -461))
    {
        printf("FABLETLC_DISPLAY_FLOAT_VIEWPORT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_FLOAT_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}
