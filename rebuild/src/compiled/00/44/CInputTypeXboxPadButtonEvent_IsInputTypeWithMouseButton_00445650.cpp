// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CInputTypeXboxPadButtonEvent.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputTypeXboxPadButtonEvent_Methods : CInputTypeXboxPadButtonEvent {
    bool Get(int a0);
};
bool CInputTypeXboxPadButtonEvent_Methods::Get(int a0) { return false; }