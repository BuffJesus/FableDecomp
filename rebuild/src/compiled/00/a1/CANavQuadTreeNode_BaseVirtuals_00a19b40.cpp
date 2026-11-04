// CANavQuadTreeNode (retail vtable 0x0129CBBC): destructor 0x00A19B40 (vtable store, tail
// jump to ~CBaseClassNonCopyable 0x0099A300), the defaulted virtuals 0x00A19B50..0x00A19BF0
// and the scalar deleting destructor 0x00A19C00 (expands the destructor in place).
// Names: FableWin.pdb 031a7620..031a79e0, same module order as retail.
#include "fable_nav_quad_tree_node.h"

CANavQuadTreeNode::~CANavQuadTreeNode() {}
const CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() const { return 0; }
CSmallVector<const CANavQuadTreeNode*, 8>* CANavQuadTreeNode::GetNeighbours() { return 0; }
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*) const { return false; }
const CNavRegion* CANavQuadTreeNode::GetRegion() const { return 0; }
unsigned char CANavQuadTreeNode::GetPreferabilityCost() const { return 0xFF; }
bool CANavQuadTreeNode::IsSwitchable() const { return false; }
bool CANavQuadTreeNode::IsSwitchableOn(unsigned __int64) const { return false; }
void CANavQuadTreeNode::SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours, CANavQuadTreeNode*) {}
void CANavQuadTreeNode::SetUpNeighbours(ENavQuadTreeNeighbours, CANavQuadTreeNode*) {}
void CANavQuadTreeNode::CalculateChildrenNeighbours() {}
void CANavQuadTreeNode::RemoveNeighbours() {}
