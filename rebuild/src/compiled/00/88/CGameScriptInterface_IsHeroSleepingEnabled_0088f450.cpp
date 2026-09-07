#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { char pad[0xd9]; unsigned char flag; };

unsigned char __fastcall CGameScriptInterface_IsHeroSleepingEnabled(const CGameScriptInterface* self)
{
    return self->World->flag;
}