// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// no cleanup -> a bare `ret`.
#include "engine/CStartupDefinitionManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CStartupDefinitionManager_Methods : CStartupDefinitionManager {
    void Init();
};
void CStartupDefinitionManager_Methods::Init() {}