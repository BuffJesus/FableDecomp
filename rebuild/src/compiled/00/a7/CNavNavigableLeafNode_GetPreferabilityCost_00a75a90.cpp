#include "engine/CNavNavigableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavNavigableLeafNode_Methods : CNavNavigableLeafNode {
    unsigned char M();
};
unsigned char CNavNavigableLeafNode_Methods::M(){ return this->PreferabilityCost; }