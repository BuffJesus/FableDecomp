#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { unsigned char _pad[0xe1]; unsigned char flagE1; };

void __fastcall CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(CGameScriptInterface* self, int /*edx*/, char v)
{
    self->World->flagE1 = v;
}