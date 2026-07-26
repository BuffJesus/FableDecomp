#include <cstdio>

struct CGameScriptInterface {
    void* vt;
    void* member14_pad[4];
    void* member14; // +0x14
};

static int g_calls = 0;
static int g_a, g_b, g_c, g_d;

struct HeroObj {
    unsigned char pad[0x91];
    unsigned char flags; // +0x91
    void GiveAbility(int a, int b, int c, int d) {
        g_calls++; g_a=a; g_b=b; g_c=c; g_d=d;
    }
};

static HeroObj g_hero;
static HeroObj* g_resolve = 0;

int __fastcall GetHeroManager(void* self) { return self ? 7 : 0; }
HeroObj* __fastcall ResolveHero(int mgr) { (void)mgr; return g_resolve; }

void __fastcall CGameScriptInterface_GiveHeroAbility(CGameScriptInterface* self, void* edx, int p1, int p2)
{
    (void)edx;
    int mgr = GetHeroManager(*(void**)((char*)self + 0x14));
    HeroObj* h = ResolveHero(mgr);
    if (h != 0 && (h->flags & 1) == 0) {
        h->GiveAbility(p1, p2, -1, 1);
    }
}

int main() {
    CGameScriptInterface obj;
    obj.member14 = (void*)1;

    // Case 1: null hero -> no call
    g_resolve = 0; g_calls = 0;
    CGameScriptInterface_GiveHeroAbility(&obj, 0, 111, 222);
    if (g_calls != 0) { std::printf("FAIL null hero\n"); return 1; }

    // Case 2: flag set -> no call
    g_hero.flags = 1; g_resolve = &g_hero; g_calls = 0;
    CGameScriptInterface_GiveHeroAbility(&obj, 0, 111, 222);
    if (g_calls != 0) { std::printf("FAIL flag set\n"); return 1; }

    // Case 3: valid -> call with proper args
    g_hero.flags = 0; g_resolve = &g_hero; g_calls = 0;
    CGameScriptInterface_GiveHeroAbility(&obj, 0, 111, 222);
    if (g_calls != 1 || g_a != 111 || g_b != 222 || g_c != -1 || g_d != 1) {
        std::printf("FAIL valid path a=%d b=%d c=%d d=%d calls=%d\n", g_a,g_b,g_c,g_d,g_calls); return 1;
    }

    std::printf("CGameScriptInterface_0088fcc0_TEST PASS\n");
    return 0;
}