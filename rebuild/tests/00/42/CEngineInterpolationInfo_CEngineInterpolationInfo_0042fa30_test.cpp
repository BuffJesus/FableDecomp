#include <stdio.h>
#include <string.h>
#include <new>

#include "fable_game.h"

// Behaviour fixture for CEngineInterpolationInfo::CEngineInterpolationInfo
// @ 0x0042FA30: every one of the 0x70 bytes (six sets, WFServerCurrentTime,
// the three-entry InterpolationSetList) ends up zero, nothing outside the
// object is touched, and the ctor returns `this`.

static unsigned char g_storage[0x80];

int main()
{
    bool ok = sizeof(CEngineInterpolationInfo) == 0x70;
    for (int fill = 0x11; fill < 0x100; fill += 0x77)
    {
        memset(g_storage, fill, sizeof(g_storage));
        CEngineInterpolationInfo* info = new (g_storage + 4) CEngineInterpolationInfo;
        ok = ok && (void*)info == (void*)(g_storage + 4);
        for (int i = 0; i < 4; ++i)
            ok = ok && g_storage[i] == (unsigned char)fill;
        for (int i = 4; i < 4 + 0x70; ++i)
            ok = ok && g_storage[i] == 0;
        for (int i = 4 + 0x70; i < 0x80; ++i)
            ok = ok && g_storage[i] == (unsigned char)fill;
        ok = ok && info->InterpolationSetList[0].WFInterpolate == info->Current.WFInterpolate;
        ok = ok && info->InterpolationSetList[1].WFInterpolate == info->PausedCurrent.WFInterpolate;
        ok = ok && info->InterpolationSetList[2].WFInterpolate == info->BulletTimeCurrent.WFInterpolate;
    }
    if (ok) printf("FABLETLC_ENGINEINTERPOLATIONINFO_CTOR_PASS\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}