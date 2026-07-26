#include <cstdio>
struct HealthObj { char pad[0x91]; unsigned char flags; char pad2[0xb4-0x92]; float health; };
struct Mid { int junk; };
struct CGameScriptInterface { char pad[0x14]; Mid* pMid; };

static HealthObj g_obj;
static Mid g_mid;
float g_defaultHealth = 100.0f;

extern "C" Mid* __fastcall get_mid(int p) { return &g_mid; }
extern "C" HealthObj* __fastcall get_hero(Mid* m) { return &g_obj; }

float __fastcall CGameScriptInterface_GetHeroHealth(CGameScriptInterface* self)
{
    HealthObj* h = get_hero((Mid*)get_mid(*(int*)((char*)self + 0x14)));
    if (h != 0 && (h->flags & 1) == 0)
        return h->health;
    return g_defaultHealth;
}

int main() {
    CGameScriptInterface self;
    g_obj.flags = 0;
    g_obj.health = 42.5f;
    float r = CGameScriptInterface_GetHeroHealth(&self);
    if (r != 42.5f) { std::printf("FAIL got %f\n", r); return 1; }
    g_obj.flags = 1;
    r = CGameScriptInterface_GetHeroHealth(&self);
    if (r != 100.0f) { std::printf("FAIL2 got %f\n", r); return 1; }
    std::printf("CGameScriptInterface_00890370_TEST PASS\n");
    return 0;
}