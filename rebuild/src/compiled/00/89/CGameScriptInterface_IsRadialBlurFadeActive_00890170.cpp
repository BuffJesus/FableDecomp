#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner { char pad[0x78]; bool flag; };
struct Mid { char pad[0x1964]; Inner* inner; };
struct CWorld { char pad[0x18]; Mid* mid; };

bool __fastcall CGameScriptInterface_IsRadialBlurFadeActive(const CGameScriptInterface* self)
{
    return self->World->mid->inner->flag;
}