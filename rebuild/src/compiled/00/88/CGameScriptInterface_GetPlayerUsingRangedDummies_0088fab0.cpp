#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern bool g_playerUsingRangedDummies;
bool __fastcall CGameScriptInterface_GetPlayerUsingRangedDummies(CGameScriptInterface* self)
{
    return g_playerUsingRangedDummies;
}