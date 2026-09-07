// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CNavBlockedQuadTreeNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavBlockedQuadTreeNode_Methods : CNavBlockedQuadTreeNode {
    bool Get(int a0, int a1);
};
bool CNavBlockedQuadTreeNode_Methods::Get(int a0, int a1) { return false; }