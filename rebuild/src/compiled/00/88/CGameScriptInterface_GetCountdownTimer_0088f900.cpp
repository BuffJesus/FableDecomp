#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { char pad[0xa0]; int frames; };

float __fastcall CGameScriptInterface_GetCountdownTimer(const CGameScriptInterface* self)
{
    return (float)self->World->frames;
}