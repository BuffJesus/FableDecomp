#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CWorld { unsigned char _pad[0xd9]; unsigned char heroSleeping; };

void __fastcall CGameScriptInterface_SetHeroSleepingAsEnabled(CGameScriptInterface* self, int edx_unused, char enabled)
{
    self->World->heroSleeping = enabled;
}

int main()
{
    CWorld inner;
    inner.heroSleeping = 0;
    CGameScriptInterface obj;
    obj.World = &inner;
    CGameScriptInterface_SetHeroSleepingAsEnabled(&obj, 0, 1);
    if (inner.heroSleeping != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetHeroSleepingAsEnabled(&obj, 0, 0);
    if (inner.heroSleeping != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088f440_TEST PASS\n");
    return 0;
}