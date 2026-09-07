// Empty member hook. __fastcall this=ecx.
#include "engine/CDefClassBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase_Methods : CDefClassBase {
    void Hook(int a0, int a1, int a2);
};
void CDefClassBase_Methods::Hook(int a0, int a1, int a2) {}