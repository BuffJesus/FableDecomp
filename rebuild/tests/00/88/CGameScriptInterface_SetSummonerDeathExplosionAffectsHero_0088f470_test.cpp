#include "engine/CGameScriptInterface.h"
#include <cstdio>
struct CWorld { unsigned char _pad[0xe1]; unsigned char flagE1; };

void __fastcall CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(CGameScriptInterface* self, int /*edx*/, char v)
{
    self->World->flagE1 = v;
}

int main()
{
    CWorld m;
    m.flagE1 = 0;
    CGameScriptInterface o;
    o.World = &m;
    CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(&o, 0, (char)1);
    if (m.flagE1 != 1) { std::printf("FAIL wrong value %d\n", (int)m.flagE1); return 1; }
    CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(&o, 0, (char)0);
    if (m.flagE1 != 0) { std::printf("FAIL wrong value2 %d\n", (int)m.flagE1); return 1; }
    std::printf("CGameScriptInterface_0088f470_TEST PASS\n");
    return 0;
}