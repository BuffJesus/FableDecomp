// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CNavSwitchableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavSwitchableLeafNode_Methods : CNavSwitchableLeafNode {
    bool Get(int a0, int a1);
};
bool CNavSwitchableLeafNode_Methods::Get(int a0, int a1) { return false; }