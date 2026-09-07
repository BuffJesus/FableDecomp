#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct HealthObj { char pad[0x91]; unsigned char flags; char pad2[0xb4-0x92]; float health; };
struct CPlayerManager { int junk; };

extern "C" CPlayerManager* __fastcall get_mid(int p);
extern "C" HealthObj* __fastcall get_hero(CPlayerManager* m);
extern float g_defaultHealth;

float __fastcall CGameScriptInterface_GetHeroHealth(CGameScriptInterface* self)
{
    HealthObj* h = get_hero((CPlayerManager*)get_mid(*(int*)((char*)self + 0x14)));
    if (h != 0 && (h->flags & 1) == 0)
        return h->health;
    return g_defaultHealth;
}