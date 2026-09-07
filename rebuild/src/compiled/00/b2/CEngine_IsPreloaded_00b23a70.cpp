#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CEngine_Methods::IsPreloaded @ 0x00b23a70
struct CEngine_Methods : CEngine {
    bool IsPreloaded();
};
bool CEngine_Methods::IsPreloaded()
{
    if (NewSceneFlag || CutscenePreloadFlag) return false;
    return true;
}