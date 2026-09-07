// Constant-return member: returns true. __fastcall this=ecx.
#include "engine/CAIStateGroup_BanditPlayDice.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BanditPlayDice_Methods : CAIStateGroup_BanditPlayDice {
    bool Get();
};
bool CAIStateGroup_BanditPlayDice_Methods::Get() { return true; }