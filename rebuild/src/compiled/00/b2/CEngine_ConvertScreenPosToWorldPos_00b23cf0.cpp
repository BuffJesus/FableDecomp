// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    bool Get(int a0, int a1);
};
bool CEngine_Methods::Get(int a0, int a1) { return false; }