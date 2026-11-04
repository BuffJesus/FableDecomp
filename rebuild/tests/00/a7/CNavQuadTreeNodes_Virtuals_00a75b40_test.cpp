// Behaviour fixture for the navigation quad-tree node overrides (provider CNavQuadTreeNodes_Virtuals_00a75b40.cpp;
// it defines none of them). Nodes are raw storage at the retail offsets the header pins and are driven by
// qualified (non-virtual) calls; RemoveRegionZero's virtual GetRegionID dispatch is byte-proven only.
#include "fable_nav_quad_tree_nodes.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct CBlockedTest : CNavBlockedQuadTreeNode
{
    const CANavQuadTreeNode* Leaf(const C2DVector& p) const { return CNavBlockedQuadTreeNode::GetLeafNodeAt(p); }
    bool Make(const CNavRegion* r, _STL::list<CANavQuadTreeNode*>& n) { return CNavBlockedQuadTreeNode::MakeRegion(r, n); }
    bool AddNav(const CNavRegion* r, _STL::list<CANavQuadTreeNode*>& n)
    {
        return CNavBlockedQuadTreeNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(r, n);
    }
    bool Zero() { return CNavBlockedQuadTreeNode::RemoveRegionZero(); }
    unsigned long Mem(unsigned long& a, unsigned long& b, unsigned long& c, unsigned long& d) const
    {
        return CNavBlockedQuadTreeNode::GetMemoryUsage(a, b, c, d);
    }
    void Index(long& i, _STL::map<const CANavQuadTreeNode*, long>& t) const { CNavBlockedQuadTreeNode::AddToIndexTable(i, t); }
};
struct CNavigableTest : CNavNavigableLeafNode
{
    const CANavQuadTreeNode* Leaf(const C2DVector& p) const { return CNavNavigableLeafNode::GetLeafNodeAt(p); }
};
struct CSwitchableTest : CNavSwitchableLeafNode
{
    bool AddNav(const CNavRegion* r, _STL::list<CANavQuadTreeNode*>& n)
    {
        return CNavSwitchableLeafNode::AddNavigableNodeNeighbouringSwitchableNodeToRegion(r, n);
    }
    bool Zero() { return CNavSwitchableLeafNode::RemoveRegionZero(); }
};

int main()
{
    static unsigned char inner[0x20], blocked[0x10], leaf[0x20], sw[0x34], region[0x14];
    memset(inner, 0xCC, sizeof(inner)); memset(blocked, 0xCC, sizeof(blocked));
    memset(leaf, 0, sizeof(leaf)); memset(sw, 0, sizeof(sw)); memset(region, 0, sizeof(region));
    const long id = 42; memcpy(region, &id, 4);
    const CNavRegion* r = reinterpret_cast<const CNavRegion*>(region);
    C2DVector pos;

    CNavQuadTreeNode* q = reinterpret_cast<CNavQuadTreeNode*>(inner);
    CHECK(!q->CNavQuadTreeNode::HasNeighbours() && q->CNavQuadTreeNode::GetRegionID() == 0);
    CHECK(q->CNavQuadTreeNode::GetNodeType() == NAV_QUAD_TREE_NON_LEAF_NODE);

    CBlockedTest* b = reinterpret_cast<CBlockedTest*>(blocked);
    CHECK(!b->CNavBlockedQuadTreeNode::HasNeighbours() && b->CNavBlockedQuadTreeNode::GetRegionID() == 0);
    CHECK(b->CNavBlockedQuadTreeNode::GetNodeType() == NAV_QUAD_TREE_BLOCKED_NODE);
    CHECK(b->Leaf(pos) == b);
    _STL::list<CANavQuadTreeNode*>* nodes = 0;
    CHECK(!b->Make(r, *nodes) && !b->AddNav(r, *nodes) && b->Zero());
    unsigned long m[4] = {1, 2, 3, 4};
    CHECK(b->Mem(m[0], m[1], m[2], m[3]) == 0 && m[0] == 1 && m[2] == 3);
    long index = 7;
    _STL::map<const CANavQuadTreeNode*, long>* table = 0;
    b->Index(index, *table);
    CHECK(index == 8);

    CNavigableTest* n = reinterpret_cast<CNavigableTest*>(leaf);
    CHECK(n->CNavNavigableLeafNode::GetNodeType() == NAV_QUAD_TREE_NAVIGABLE_NODE && n->Leaf(pos) == n);
    CHECK((void*)n->CNavNavigableLeafNode::GetNeighbours() == leaf + 0x14);
    CHECK((const void*)static_cast<const CNavNavigableLeafNode*>(n)->CNavNavigableLeafNode::GetNeighbours() == leaf + 0x14);

    CSwitchableTest* s = reinterpret_cast<CSwitchableTest*>(sw);
    memcpy(sw + 0x10, &r, 4); sw[0x18] = 2; sw[0x1C] = 5; sw[0x0F] = 0;           // switched on
    CHECK(s->CNavSwitchableLeafNode::HasNeighbours() && s->CNavSwitchableLeafNode::GetRegionID() == 42);
    CHECK(s->CNavSwitchableLeafNode::GetRegion() == r && s->CNavSwitchableLeafNode::GetPreferabilityCost() == 5);
    sw[0x0F] = 1;                                                                   // switched off
    CHECK(!s->CNavSwitchableLeafNode::HasNeighbours() && s->CNavSwitchableLeafNode::GetRegionID() == 0);
    CHECK(s->CNavSwitchableLeafNode::GetRegion() == 0 && s->CNavSwitchableLeafNode::GetPreferabilityCost() == 0xFF);
    sw[0x0F] = 0; memset(sw + 0x10, 0, 4);                                         // on, but no region
    CHECK(s->CNavSwitchableLeafNode::GetRegionID() == 0);
    CHECK(s->CNavSwitchableLeafNode::GetNodeType() == NAV_QUAD_TREE_SWITCHABLE_NODE && s->CNavSwitchableLeafNode::IsSwitchable());
    CHECK(!s->AddNav(r, *nodes) && !s->Zero());

    printf(g_fail ? "FAIL\n" : "NAV_QUAD_TREE_NODES_OK\n");
    return g_fail != 0;
}
