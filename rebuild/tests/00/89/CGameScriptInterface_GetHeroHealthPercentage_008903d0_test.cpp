#include <cstdio>

struct CThing { char pad[0x92]; };
struct CGameScriptInterface { char pad[0x14]; void* field14; };

static CThing g_hero;
static void* g_mgr = (void*)0x1234;
float g_defaultHealthPct = -1.0f;

void* __fastcall Sub_GetManager(void* self) { return self; }
CThing* __fastcall Sub_ResolveHero(void* mgr) { return mgr == g_mgr ? &g_hero : (CThing*)0; }
float __fastcall Sub_GetHealthPct(CThing* hero) { return 0.75f; }

float __fastcall CGameScriptInterface_GetHeroHealthPercentage(CGameScriptInterface* self)
{
    void* mgr = self->field14;
    CThing* hero = Sub_ResolveHero(Sub_GetManager(mgr));
    if (hero != 0 && (*(unsigned char*)((char*)hero + 0x91) & 0x1) == 0) {
        float v = Sub_GetHealthPct(hero);
        return v;
    }
    return g_defaultHealthPct;
}

int main() {
    CGameScriptInterface gsi;
    gsi.field14 = g_mgr;
    *(unsigned char*)((char*)&g_hero + 0x91) = 0;
    float r = CGameScriptInterface_GetHeroHealthPercentage(&gsi);
    if (r != 0.75f) { std::printf("FAIL alive path %f\n", r); return 1; }
    *(unsigned char*)((char*)&g_hero + 0x91) = 1;
    r = CGameScriptInterface_GetHeroHealthPercentage(&gsi);
    if (r != -1.0f) { std::printf("FAIL flag path %f\n", r); return 1; }
    gsi.field14 = (void*)0x9999;
    r = CGameScriptInterface_GetHeroHealthPercentage(&gsi);
    if (r != -1.0f) { std::printf("FAIL null path %f\n", r); return 1; }
    std::printf("CGameScriptInterface_008903d0_TEST PASS\n");
    return 0;
}