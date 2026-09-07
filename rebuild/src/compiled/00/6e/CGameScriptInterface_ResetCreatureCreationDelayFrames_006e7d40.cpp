
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->CameraResetToViewBehindHeroCount = 0;
}