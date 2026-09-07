// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CIDrawMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIDrawMap_Methods : CIDrawMap {
    bool Get(int a0, int a1, int a2);
};
bool CIDrawMap_Methods::Get(int a0, int a1, int a2) { return false; }