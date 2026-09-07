#include "engine/CEngineSurfaceCompositionManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineSurfaceCompositionManager_Methods : CEngineSurfaceCompositionManager {
    unsigned long __fastcall PeekSceneFilterFlags();
};

unsigned long __fastcall CEngineSurfaceCompositionManager_Methods::PeekSceneFilterFlags()
{
    return 3;
}