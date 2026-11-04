#pragma once

// CNavWayPoint: one way point of the A* navigator route (CNavigatorAStar::Route,
// _STL::list<CNavWayPoint> at retail +0x8C, fable_navigator.h).
// Members: Ego_r.pdb (Position +0x0, NavLayer +0x8, Removable +0x9), sizeof 0xA.
// Where the 0xA comes from: Ego_r does NOT pack CNavWayPoint itself (DIA packed flag clear);
// it packs C2DVector (DIA packed flag set, alignment 1), so CNavWayPoint has alignment 1 and
// no tail padding. rebuild/include/fable_2d_vector.h does not model that C2DVector packing
// (changing it is a separate, wide-blast-radius item: every C2DVector user), so this header
// packs CNavWayPoint locally as an INTERIM stand-in. It gives the identical layout (+0x0 /
// +0x8 / +0x9, sizeof 0xA, alignment 1); drop the pragma once C2DVector is packed.
// Retail agrees: every list<CNavWayPoint> node is 0x14 bytes (ctor 0x00A21980; node copies
// 0x00A1CD60 / 0x00A21561 / 0x00A21FC6 write +0x0, +0x4, then the two bytes +0x8 / +0x9),
// i.e. the 8-byte node links plus 0xA bytes rounded up to the node's 4-byte alignment.
// Ego_r also has inline constructors CNavWayPoint() and
// CNavWayPoint(const C2DVector&, unsigned char, bool); not modelled (bodies unrecovered).
// Trivially destructible: list<CNavWayPoint>::pop_front 0x00A219E0 and ~list 0x00A214C0
// only unhook and free nodes.

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_stlport_list.h"   // _List_node (node-size assert)

#pragma pack(push,1)   // interim: stands in for Ego_r's packed C2DVector (see above)
class CNavWayPoint
{
public:
    C2DVector     Position;   // +0x0
    unsigned char NavLayer;   // +0x8
    bool          Removable;  // +0x9
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CNavWayPoint) == 0xA);
FABLE_STATIC_ASSERT(offsetof(CNavWayPoint, NavLayer) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CNavWayPoint, Removable) == 0x9);
FABLE_STATIC_ASSERT(sizeof(_STL::_List_node<CNavWayPoint>) == 0x14);  // retail node size
