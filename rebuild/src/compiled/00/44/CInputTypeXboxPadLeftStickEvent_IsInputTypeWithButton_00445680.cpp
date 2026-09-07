// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CInputTypeXboxPadLeftStickEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeXboxPadLeftStickEvent_Methods : CInputTypeXboxPadLeftStickEvent {
    bool Get(int a0);
};
bool CInputTypeXboxPadLeftStickEvent_Methods::Get(int a0) { return false; }