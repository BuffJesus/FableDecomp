#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDisplayEngine { void EndLetterBox(); };
extern "C" void __fastcall CGameScriptInterface_EndLetterBox(CGameScriptInterface* self)
{
    self->DisplayEngine->EndLetterBox();
}