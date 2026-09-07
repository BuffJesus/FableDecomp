// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// no cleanup -> a bare `ret`.
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void Init();
};
void CDefClassBase_Methods::Init() {}