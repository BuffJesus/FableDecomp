#pragma once

// CANavQuadTreeNode: abstract navigation quad-tree node.
// Ancestry: retail RTTI CANavQuadTreeNode : CBaseClassNonCopyable : CBaseClass.
// Retail vtable 0x0129CBBC (23 slots). Slot order and names: FableWin.pdb
// CNavSwitchableLeafNode vtable (every slot resolved through its incremental-link
// thunk) cross-checked against Ego_r.pdb CNavNavigableLeafNode; the defaulted slots
// match retail 0x00A19B50..0x00A19BF0 one for one. Data members: Ego_r.pdb
// (sizeof 0x10; no container members, so retail and Ego_r agree).
//
// Do not include together with the generated POD view engine/CANavQuadTreeNode.h
// (same class name).

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_base_class.h"
#include "fable_small_vector.h"
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"   // _STL::map

class CAFile;
class CNavRegion;

// Ego_r enum ENavQuadTreeNeighbours / ENavQuadTreeNodeType (enumerators not needed).
enum ENavQuadTreeNeighbours
{
};
enum ENavQuadTreeNodeType
{
    NAV_QUAD_TREE_NON_LEAF_NODE = 0,
    NAV_QUAD_TREE_NAVIGABLE_NODE = 1,
    NAV_QUAD_TREE_BLOCKED_NODE = 2,
    NAV_QUAD_TREE_SWITCHABLE_NODE = 3
};

class CANavQuadTreeNode : public CBaseClassNonCopyable
{
public:
    virtual ~CANavQuadTreeNode();                                                           // 0 (??1 0x00A19B40, ??_G 0x00A19C00)
    // VC7.1 places overloaded virtuals in reverse declaration order (slots 2, 1).
    virtual const CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours() const;         // 2 (0x00A19B50)
    virtual CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours();                     // 1 (0x00A19B60)
    virtual bool HasNeighbours() const = 0;                                                 // 3
    virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;                        // 4 (0x00A19B70)
    virtual long GetRegionID() const = 0;                                                   // 5
    virtual ENavQuadTreeNodeType GetNodeType() const = 0;                                   // 6
    virtual const CNavRegion* GetRegion() const;                                            // 7 (0x00A19B80)
    virtual unsigned char GetPreferabilityCost() const;                                     // 8 (0x00A19B90)
    virtual bool IsSwitchable() const;                                                      // 9 (0x00A19BA0)
    virtual bool IsSwitchableOn(unsigned __int64 switches) const;                           // 10 (0x00A19BB0)

protected:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector& pos) const = 0;         // 11
    virtual bool MakeRegion(const CNavRegion* region,
                            _STL::list<CANavQuadTreeNode*>& nodes) = 0;                     // 12
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(
        const CNavRegion* region, _STL::list<CANavQuadTreeNode*>& nodes) = 0;               // 13
    virtual bool RemoveRegionZero() = 0;                                                    // 14
    virtual unsigned long GetMemoryUsage(unsigned long& a, unsigned long& b,
                                         unsigned long& c, unsigned long& d) const = 0;     // 15
    virtual void SetUpNeighboursWithLeaf(ENavQuadTreeNeighbours direction,
                                         CANavQuadTreeNode* leaf);                          // 16 (0x00A19BC0)
    virtual void SetUpNeighbours(ENavQuadTreeNeighbours direction, CANavQuadTreeNode* node);  // 17 (0x00A19BD0)
    virtual void CalculateChildrenNeighbours();                                             // 18 (0x00A19BE0)
    virtual void RemoveNeighbours();                                                        // 19 (0x00A19BF0)
    virtual void AddToIndexTable(long& index,
                                 _STL::map<const CANavQuadTreeNode*, long>& table) const = 0;  // 20
    virtual void SaveToFile(CAFile& file, const _STL::map<const CANavQuadTreeNode*, long>& table,
                            bool flag, const C2DVector& origin) const = 0;                  // 21
    virtual void ConvertIndexesToPointers(const _STL::map<long, CANavQuadTreeNode*>& table) = 0;  // 22

public:
    C2DVector     Position;       // +0x04
    unsigned char QuadTreeLevel;  // +0x0C
    unsigned char NavLayer;       // +0x0D
    bool          LeafNode;       // +0x0E
    bool          Blocked;        // +0x0F
};

FABLE_STATIC_ASSERT(sizeof(CANavQuadTreeNode) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Position) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, QuadTreeLevel) == 0xC);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Blocked) == 0xF);
