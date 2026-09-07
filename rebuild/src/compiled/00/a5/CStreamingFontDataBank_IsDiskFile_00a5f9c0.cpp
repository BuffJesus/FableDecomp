// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CStreamingFontDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CStreamingFontDataBank_Methods : CStreamingFontDataBank {
    bool Get(int a0, int a1);
};
bool CStreamingFontDataBank_Methods::Get(int a0, int a1) { return false; }