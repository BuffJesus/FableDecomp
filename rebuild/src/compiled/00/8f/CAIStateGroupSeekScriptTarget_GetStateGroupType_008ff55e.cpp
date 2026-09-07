#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_SeekScriptTarget.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_SeekScriptTarget_Methods : CAIStateGroup_SeekScriptTarget {
    int Get();
};
int CAIStateGroup_SeekScriptTarget_Methods::Get() { return 1; }