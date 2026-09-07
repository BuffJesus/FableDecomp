// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CFontDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CFontDataBank_Methods : CFontDataBank {
    bool Get(int a0, int a1);
};
bool CFontDataBank_Methods::Get(int a0, int a1) { return false; }