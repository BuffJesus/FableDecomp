// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CAIGameCameraBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIGameCameraBase_Methods : CAIGameCameraBase {
    bool Get(int a0, int a1);
};
bool CAIGameCameraBase_Methods::Get(int a0, int a1) { return false; }