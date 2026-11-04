#pragma once
// Hand-owned (retail model): the concrete navigation quad-tree nodes over CANavQuadTreeNode
// (fable_nav_quad_tree_node.h). Ego_r.pdb names, members and access; retail RTTI vtables:
//   CNavQuadTreeNode        0x0129DCD4 (0x20: TL/TR/BL/BR child nodes +0x10..+0x1C)
//   CNavBlockedQuadTreeNode 0x0129DD3C (0x10, no members)
//   CNavNavigableLeafNode   0x0129DDA4 (0x20: PRegion +0x10, Neighbours +0x14, PreferabilityCost +0x1C)
//   CNavSwitchableLeafNode  0x0129DE04 (0x34: CNavNavigableLeafNode + UniqueIDsOpen +0x20, not modelled yet)
// Retail emits these small virtuals as COMDATs beside each vtable; the navigable getters the switchable node
// reuses stay inline here (its overrides read the fields directly), the rest are defined out of line.
// The engine/CNav*Node.h generated views are shims of this header.
#include "fable_nav_quad_tree_node.h"

// Ego_r CNavRegion 0x14: RegionID +0x0, Connections, MostRecentSearchID. Partial view (no size assert).
class CNavRegion
{
public:
    long GetRegionID() const { return RegionID; }

private:
    long RegionID;   // +0x00
};

class CNavQuadTreeNode : public CANavQuadTreeNode
{
public:
    virtual bool HasNeighbours() const;
    virtual long GetRegionID() const;
    virtual ENavQuadTreeNodeType GetNodeType() const;

private:
    friend struct CNavQuadTreeNodesLayoutCheck;
    CANavQuadTreeNode* TLNode;   // +0x10
    CANavQuadTreeNode* TRNode;   // +0x14
    CANavQuadTreeNode* BLNode;   // +0x18
    CANavQuadTreeNode* BRNode;   // +0x1C
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode
{
public:
    virtual bool HasNeighbours() const;
    virtual long GetRegionID() const;
    virtual ENavQuadTreeNodeType GetNodeType() const;

protected:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;
    virtual bool MakeRegion(const CNavRegion* region, _STL::list<CANavQuadTreeNode*>& nodes);
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(
        const CNavRegion* region, _STL::list<CANavQuadTreeNode*>& nodes);
    virtual bool RemoveRegionZero();
    virtual unsigned long GetMemoryUsage(unsigned long& a, unsigned long& b,
                                         unsigned long& c, unsigned long& d) const;
    virtual void AddToIndexTable(long& index, _STL::map<const CANavQuadTreeNode*, long>& table) const;
    virtual void ConvertIndexesToPointers(const _STL::map<long, CANavQuadTreeNode*>& table);
};

class CNavNavigableLeafNode : public CANavQuadTreeNode
{
public:
    virtual bool HasNeighbours() const { return Neighbours.Size != 0; }
    virtual long GetRegionID() const { return PRegion ? PRegion->GetRegionID() : 0; }
    virtual const CNavRegion* GetRegion() const { return PRegion; }
    virtual CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours();
    virtual const CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours() const;
    virtual ENavQuadTreeNodeType GetNodeType() const;
    virtual unsigned char GetPreferabilityCost() const { return PreferabilityCost; }

protected:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const;
    virtual bool RemoveRegionZero();
    virtual unsigned long GetMemoryUsage(unsigned long& a, unsigned long& b,
                                         unsigned long& c, unsigned long& d) const;

    const CNavRegion* PRegion;                               // +0x10

private:
    friend struct CNavQuadTreeNodesLayoutCheck;
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;    // +0x14
    unsigned char PreferabilityCost;                         // +0x1C
};

// A navigable leaf whose passability follows switches; while switched off (Blocked) it has no region,
// no neighbours and the worst preferability cost.
class CNavSwitchableLeafNode : public CNavNavigableLeafNode
{
public:
    virtual bool HasNeighbours() const;
    virtual long GetRegionID() const;
    virtual const CNavRegion* GetRegion() const;
    virtual ENavQuadTreeNodeType GetNodeType() const;
    virtual unsigned char GetPreferabilityCost() const;
    virtual bool IsSwitchable() const;

protected:
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(
        const CNavRegion* region, _STL::list<CANavQuadTreeNode*>& nodes);
    virtual bool RemoveRegionZero();
};

struct CNavQuadTreeNodesLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeNode, TLNode) == 0x10);
    FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeNode, BRNode) == 0x1C);
    FABLE_STATIC_ASSERT(offsetof(CNavNavigableLeafNode, Neighbours) == 0x14);
    FABLE_STATIC_ASSERT(offsetof(CNavNavigableLeafNode, PreferabilityCost) == 0x1C);
};
FABLE_STATIC_ASSERT(sizeof(CNavQuadTreeNode) == 0x20);
FABLE_STATIC_ASSERT(sizeof(CNavBlockedQuadTreeNode) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CNavNavigableLeafNode) == 0x20);
