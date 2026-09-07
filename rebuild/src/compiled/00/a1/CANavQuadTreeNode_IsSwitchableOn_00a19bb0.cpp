// Constant-return member: returns false. __fastcall this=ecx.
#include "engine/CANavQuadTreeNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CANavQuadTreeNode_Methods : CANavQuadTreeNode {
    bool Get(int a0, int a1);
};
bool CANavQuadTreeNode_Methods::Get(int a0, int a1) { return false; }