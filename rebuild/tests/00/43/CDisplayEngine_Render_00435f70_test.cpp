// Behaviour: Render forwards (info, worldUpdated, present) unchanged to DoRender on
// the same object, once, for every flag combination.
#include "fable_display_engine.h"
#include <stdio.h>

static int g_calls; static const void* g_this; static const CInterpolationInfo* g_info; static bool g_a, g_b;
void CDisplayEngine::DoRender(const CInterpolationInfo* info, bool worldUpdated, bool present)
{ ++g_calls; g_this = this; g_info = info; g_a = worldUpdated; g_b = present; }

int main()
{
    unsigned char storage[sizeof(CDisplayEngine)];
    CDisplayEngine* engine = reinterpret_cast<CDisplayEngine*>(storage);
    const CInterpolationInfo* info = reinterpret_cast<const CInterpolationInfo*>(0x1234);
    for (int m = 0; m < 4; ++m)
    {
        g_calls = 0; g_this = 0; g_info = 0; g_a = !(m & 1); g_b = !(m & 2);
        engine->Render(info, (m & 1) != 0, (m & 2) != 0);
        if (g_calls != 1 || g_this != engine || g_info != info || g_a != ((m & 1) != 0) || g_b != ((m & 2) != 0)) { printf("FAIL %d\n", m); return 1; }
    }
    printf("PASS_00435f70\n");
    return 0;
}