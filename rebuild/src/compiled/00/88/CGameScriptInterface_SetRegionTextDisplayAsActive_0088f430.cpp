#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld { unsigned char _pad[0xdf]; unsigned char regionTextActive; };

void __fastcall CGameScriptInterface_SetRegionTextDisplayAsActive(CGameScriptInterface* self, void* edx, char active)
{
    self->World->regionTextActive = (unsigned char)active;
}