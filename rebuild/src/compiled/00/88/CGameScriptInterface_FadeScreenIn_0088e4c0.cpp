#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDisplayEngine { char pad[0xd8]; unsigned char flag; };
extern void __fastcall CGameScriptInterface_DoFade(CGameScriptInterface* self);
void __fastcall CGameScriptInterface_FadeScreenIn(CGameScriptInterface* self)
{
    self->DisplayEngine->flag = 0;
    CGameScriptInterface_DoFade(self);
}