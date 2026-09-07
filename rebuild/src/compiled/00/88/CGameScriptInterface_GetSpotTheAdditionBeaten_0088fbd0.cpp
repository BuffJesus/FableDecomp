#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern bool g_SpotTheAdditionBeaten;


bool __fastcall CGameScriptInterface_GetSpotTheAdditionBeaten(const CGameScriptInterface* self)
{
    (void)self;
    return g_SpotTheAdditionBeaten;
}