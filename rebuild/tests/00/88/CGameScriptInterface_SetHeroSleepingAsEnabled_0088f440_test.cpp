#include <cstdio>

struct CMid { unsigned char _pad[0xd9]; unsigned char heroSleeping; };
struct CGameScriptInterface { void* vt; CMid* mid; };

void __fastcall CGameScriptInterface_SetHeroSleepingAsEnabled(CGameScriptInterface* self, int edx_unused, char enabled)
{
    self->mid->heroSleeping = enabled;
}

int main()
{
    CMid inner;
    inner.heroSleeping = 0;
    CGameScriptInterface obj;
    obj.mid = &inner;
    CGameScriptInterface_SetHeroSleepingAsEnabled(&obj, 0, 1);
    if (inner.heroSleeping != 1) { std::printf("FAIL set 1\n"); return 1; }
    CGameScriptInterface_SetHeroSleepingAsEnabled(&obj, 0, 0);
    if (inner.heroSleeping != 0) { std::printf("FAIL set 0\n"); return 1; }
    std::printf("CGameScriptInterface_0088f440_TEST PASS\n");
    return 0;
}