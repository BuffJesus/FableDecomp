
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct HeroObj {
    unsigned char pad[0x91];
    unsigned char flags; // +0x91
    void GiveAbility(int a, int b, int c, int d);
};

extern int __fastcall GetHeroManager(void* self);
extern HeroObj* __fastcall ResolveHero(int mgr);

void __fastcall CGameScriptInterface_GiveHeroAbility(CGameScriptInterface* self, void* edx, int p1, int p2)
{
    int mgr = GetHeroManager(*(void**)((char*)self + 0x14));
    HeroObj* h = ResolveHero(mgr);
    if (h != 0 && (h->flags & 1) == 0) {
        h->GiveAbility(p1, p2, -1, 1);
    }
}