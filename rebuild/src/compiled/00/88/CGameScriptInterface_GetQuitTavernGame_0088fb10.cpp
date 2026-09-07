#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern bool g_QuitTavernGame;


bool __fastcall CGameScriptInterface_GetQuitTavernGame(CGameScriptInterface* self)
{
    (void)self;
    return g_QuitTavernGame;
}