#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { char pad[0x160]; float angle; };

float __fastcall CGameScriptInterface_GetGuildSealRecallAngleXY(const CGameScriptInterface* self)
{
    return self->World->angle;
}