#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGameScriptInterface_Methods : CGameScriptInterface {
    // +0x4c
    void SetCreatureCreationDelayFrames(int frames);
};

void CGameScriptInterface_Methods::SetCreatureCreationDelayFrames(int frames)
{
    this->CameraResetToViewBehindHeroCount = frames;
}