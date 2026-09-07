#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CutsceneMgr { char pad[0xf6]; unsigned char inCutscene; };
extern CutsceneMgr* g_cutsceneMgr;
char __fastcall CGameScriptInterface_IsInCutscene(const CGameScriptInterface* self)
{
    return (char)g_cutsceneMgr->inCutscene;
}