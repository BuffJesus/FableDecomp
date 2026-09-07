#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Hero {
    char pad[0x91];
    unsigned char flags;
};

struct CPlayerManager { int dummy; };

extern "C" CPlayerManager* __fastcall GSI_GetWorld(CPlayerManager* w);
extern "C" Hero*  __fastcall World_GetHero(CPlayerManager* w);
extern "C" bool   __fastcall Hero_CheckChild(Hero* h);

bool __fastcall CGameScriptInterface_IsHeroChild(CGameScriptInterface* self)
{
    Hero* h = World_GetHero((CPlayerManager*)GSI_GetWorld(self->PlayerManager));
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}