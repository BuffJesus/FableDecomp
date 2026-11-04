// Small virtual overrides of the concrete navigation quad-tree nodes. Retail emits them as COMDATs beside each
// vtable (0x00A75B40.., 0x00A76270.., 0x00A76500.., 0x00A7B930.., 0x00A7D920.., 0x00A7F290.., 0x00A84E00..).
#include "fable_nav_quad_tree_nodes.h"

// ---- CNavQuadTreeNode (an inner node: never a leaf, never in a region)
bool CNavQuadTreeNode::HasNeighbours() const { return false; }                                  // 0x00A7B930
long CNavQuadTreeNode::GetRegionID() const { return 0; }                                       // 0x00A7B940
ENavQuadTreeNodeType CNavQuadTreeNode::GetNodeType() const { return NAV_QUAD_TREE_NON_LEAF_NODE; }  // 0x00A7B950

// ---- CNavBlockedQuadTreeNode (an impassable leaf)
bool CNavBlockedQuadTreeNode::HasNeighbours() const { return false; }                           // 0x00A7D920
long CNavBlockedQuadTreeNode::GetRegionID() const { return 0; }                                // 0x00A7D930
ENavQuadTreeNodeType CNavBlockedQuadTreeNode::GetNodeType() const { return NAV_QUAD_TREE_BLOCKED_NODE; }  // 0x00A7D940
const CANavQuadTreeNode* CNavBlockedQuadTreeNode::GetLeafNodeAt(const C2DVector&) const { return this; }  // 0x00A76270
bool CNavBlockedQuadTreeNode::MakeRegion(const CNavRegion*, _STL::list<CANavQuadTreeNode*>&) { return false; }  // 0x00A7D950
bool CNavBlockedQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(
    const CNavRegion*, _STL::list<CANavQuadTreeNode*>&) { return false; }                     // 0x00A7D960
bool CNavBlockedQuadTreeNode::RemoveRegionZero() { return true; }                               // 0x00A7D970
unsigned long CNavBlockedQuadTreeNode::GetMemoryUsage(unsigned long&, unsigned long&,
                                                      unsigned long&, unsigned long&) const { return 0; }  // 0x00A76290
void CNavBlockedQuadTreeNode::AddToIndexTable(long& index, _STL::map<const CANavQuadTreeNode*, long>&) const
{
    ++index;                                                                                   // 0x00A76280
}
void CNavBlockedQuadTreeNode::ConvertIndexesToPointers(const _STL::map<long, CANavQuadTreeNode*>&) {}  // 0x00A7D980

// ---- CNavNavigableLeafNode
CSmallVector<const CANavQuadTreeNode*, 8>* CNavNavigableLeafNode::GetNeighbours() { return &Neighbours; }  // 0x00A7F2A0
const CSmallVector<const CANavQuadTreeNode*, 8>* CNavNavigableLeafNode::GetNeighbours() const { return &Neighbours; }  // 0x00A7F290
ENavQuadTreeNodeType CNavNavigableLeafNode::GetNodeType() const { return NAV_QUAD_TREE_NAVIGABLE_NODE; }  // 0x00A7F2B0
const CANavQuadTreeNode* CNavNavigableLeafNode::GetLeafNodeAt(const C2DVector&) const { return this; }  // 0x00A75B40
bool CNavNavigableLeafNode::RemoveRegionZero() { return GetRegionID() == 0; }                   // 0x00A75B50
// 0x00A76500: counts this leaf in the first and third totals; its size includes the neighbour list.
unsigned long CNavNavigableLeafNode::GetMemoryUsage(unsigned long& a, unsigned long&,
                                                    unsigned long& c, unsigned long&) const
{
    ++a;
    ++c;
    return sizeof(CNavNavigableLeafNode) + Neighbours.Size * sizeof(const CANavQuadTreeNode*);
}

// ---- CNavSwitchableLeafNode
bool CNavSwitchableLeafNode::HasNeighbours() const                                             // 0x00A76530
{
    if (!Blocked)
        return CNavNavigableLeafNode::HasNeighbours();
    return false;
}
long CNavSwitchableLeafNode::GetRegionID() const { return Blocked ? 0 : CNavNavigableLeafNode::GetRegionID(); }  // 0x00A75BE0
const CNavRegion* CNavSwitchableLeafNode::GetRegion() const                                     // 0x00A75C00
{
    if (!Blocked)
        return CNavNavigableLeafNode::GetRegion();
    return 0;
}
unsigned char CNavSwitchableLeafNode::GetPreferabilityCost() const
{
    if (!Blocked)                                                                               // 0x00A75C10
        return CNavNavigableLeafNode::GetPreferabilityCost();
    return 0xFF;
}
ENavQuadTreeNodeType CNavSwitchableLeafNode::GetNodeType() const { return NAV_QUAD_TREE_SWITCHABLE_NODE; }  // 0x00A84E00
bool CNavSwitchableLeafNode::IsSwitchable() const { return true; }                              // 0x00A84E10
bool CNavSwitchableLeafNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(
    const CNavRegion*, _STL::list<CANavQuadTreeNode*>&) { return false; }                     // 0x00A84E20
bool CNavSwitchableLeafNode::RemoveRegionZero() { return false; }                               // 0x00A84E30
