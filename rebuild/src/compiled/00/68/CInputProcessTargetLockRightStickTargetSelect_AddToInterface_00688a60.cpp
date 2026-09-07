#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputProcessTargetLockRightStickTargetSelect_Methods : CInputProcessTargetLockRightStickTargetSelect {
    void AddToInterface();
};
extern void __fastcall base_ati(CInputProcessTargetLockRightStickTargetSelect*);
void CInputProcessTargetLockRightStickTargetSelect_Methods::AddToInterface(){ this->ChangedTargetRight = 0; this->ChangedTargetLeft = 0; base_ati(this); }