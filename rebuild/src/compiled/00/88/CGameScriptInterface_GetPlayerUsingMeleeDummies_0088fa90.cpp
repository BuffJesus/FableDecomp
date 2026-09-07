#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern bool g_PlayerUsingMeleeDummies;
bool __fastcall CGameScriptInterface_GetPlayerUsingMeleeDummies(const CGameScriptInterface* self)
{
    (void)self;
    return g_PlayerUsingMeleeDummies;
}