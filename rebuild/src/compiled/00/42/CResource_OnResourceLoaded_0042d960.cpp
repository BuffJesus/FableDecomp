// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// no cleanup -> a bare `ret`.
#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    void Init();
};
void CResource_Methods::Init() {}