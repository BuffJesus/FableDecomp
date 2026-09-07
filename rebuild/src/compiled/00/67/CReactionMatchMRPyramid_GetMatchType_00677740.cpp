// Constant-return member: returns 0x3. __fastcall this=ecx.
#include "engine/CReactionMatch_MRPyramid.h"  // retyped onto the PDB layout; byte parity re-verified
struct CReactionMatch_MRPyramid_Methods : CReactionMatch_MRPyramid {
    int Get();
};
int CReactionMatch_MRPyramid_Methods::Get() { return 0x3; }