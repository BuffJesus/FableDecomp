#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct Hero {
    char pad[0x91];
    unsigned char flags;
};

struct CPlayerManager { int dummy; };

static Hero g_hero;
static CPlayerManager g_world;
static int g_checkResult = 1;

extern "C" CPlayerManager* __fastcall GSI_GetWorld(CPlayerManager* w) { return &g_world; }
extern "C" Hero*  __fastcall World_GetHero(CPlayerManager* w) { return &g_hero; }
extern "C" bool   __fastcall Hero_CheckChild(Hero* h) { return g_checkResult != 0; }

bool __fastcall CGameScriptInterface_IsHeroChild(CGameScriptInterface* self)
{
    Hero* h = World_GetHero((CPlayerManager*)GSI_GetWorld(self->PlayerManager));
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}

int main()
{
    CGameScriptInterface self;
    self.PlayerManager = &g_world;

    // Case 1: flags bit0 clear, check returns true => true
    g_hero.flags = 0;
    g_checkResult = 1;
    if (CGameScriptInterface_IsHeroChild(&self) != true) { std::printf("FAIL case1\n"); return 1; }

    // Case 2: flags bit0 set => false regardless
    g_hero.flags = 1;
    g_checkResult = 1;
    if (CGameScriptInterface_IsHeroChild(&self) != false) { std::printf("FAIL case2\n"); return 1; }

    // Case 3: flags clear, check false => false
    g_hero.flags = 0;
    g_checkResult = 0;
    if (CGameScriptInterface_IsHeroChild(&self) != false) { std::printf("FAIL case3\n"); return 1; }

    std::printf("CGameScriptInterface_008902b0_TEST PASS\n");
    return 0;
}