// Constant-true predicate: `mov al,1; ret`. __fastcall this=ecx, no args.
#include "engine/CParentDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CParentDefClassBase_Methods : CParentDefClassBase {
    bool OnPostDeviceReset();
};
bool CParentDefClassBase_Methods::OnPostDeviceReset() { return true; }