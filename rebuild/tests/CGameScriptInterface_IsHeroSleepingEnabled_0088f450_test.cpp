#include <cstdio>

struct Inner { char pad[0xd9]; unsigned char flag; };
struct CGameScriptInterface { void* vtbl; Inner* inner; };

unsigned char __fastcall CGameScriptInterface_IsHeroSleepingEnabled(const CGameScriptInterface* self)
{
    return self->inner->flag;
}

int main()
{
    Inner in1; in1.flag = 0;
    Inner in2; in2.flag = 7;
    CGameScriptInterface a; a.inner = &in1;
    CGameScriptInterface b; b.inner = &in2;
    if (CGameScriptInterface_IsHeroSleepingEnabled(&a) != 0) { std::printf("FAIL a\n"); return 1; }
    if (CGameScriptInterface_IsHeroSleepingEnabled(&b) != 7) { std::printf("FAIL b\n"); return 1; }
    std::printf("CGameScriptInterface_0088f450_TEST PASS\n");
    return 0;
}