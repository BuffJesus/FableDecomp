#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { unsigned char pad[0xd7]; unsigned char flagD7; };

void __fastcall CGameScriptInterface_SetSavingAsEnabled(CGameScriptInterface* self, int edx_dummy, char enabled)
{
    self->World->flagD7 = (unsigned char)enabled;
}