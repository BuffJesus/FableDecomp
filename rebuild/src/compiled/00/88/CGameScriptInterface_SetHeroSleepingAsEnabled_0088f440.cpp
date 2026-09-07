#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { unsigned char _pad[0xd9]; unsigned char heroSleeping; };

void __fastcall CGameScriptInterface_SetHeroSleepingAsEnabled(CGameScriptInterface* self, int edx_unused, char enabled)
{
    self->World->heroSleeping = enabled;
}