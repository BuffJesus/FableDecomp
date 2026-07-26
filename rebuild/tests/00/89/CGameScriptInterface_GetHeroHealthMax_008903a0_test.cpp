#include <cstdio>

struct CGameScriptInterface {
    char pad[0x14];
    void* member14;
};

struct FakeHero {
    char pad[0x91];
    unsigned char flag;   // +0x91
    char pad2[0xb0 - 0x92];
    float health;         // +0xb0
};

FakeHero g_hero;
float g_defaultHealthMax = 42.0f;
FakeHero* g_resolve = 0;

extern "C" void* __fastcall GetThing(void* p) { return p; }
extern "C" FakeHero* __fastcall ResolveHero(void* p) { (void)p; return g_resolve; }

float __fastcall CGameScriptInterface_GetHeroHealthMax(CGameScriptInterface* self)
{
    void* t = GetThing(self->member14);
    FakeHero* h = ResolveHero(t);
    if (h != 0) {
        if ((*(unsigned char*)((char*)h + 0x91) & 1) == 0) {
            return *(float*)((char*)h + 0xb0);
        }
    }
    return g_defaultHealthMax;
}

int main() {
    CGameScriptInterface gsi;
    gsi.member14 = (void*)0x1;

    // Case 1: hero null -> default
    g_resolve = 0;
    float r1 = CGameScriptInterface_GetHeroHealthMax(&gsi);
    if (r1 != 42.0f) { std::printf("FAIL case1 %f\n", r1); return 1; }

    // Case 2: hero valid, flag clear -> health
    g_hero.flag = 0;
    g_hero.health = 123.5f;
    g_resolve = &g_hero;
    float r2 = CGameScriptInterface_GetHeroHealthMax(&gsi);
    if (r2 != 123.5f) { std::printf("FAIL case2 %f\n", r2); return 1; }

    // Case 3: hero valid, flag bit0 set -> default
    g_hero.flag = 1;
    float r3 = CGameScriptInterface_GetHeroHealthMax(&gsi);
    if (r3 != 42.0f) { std::printf("FAIL case3 %f\n", r3); return 1; }

    std::printf("CGameScriptInterface_008903a0_TEST PASS\n");
    return 0;
}