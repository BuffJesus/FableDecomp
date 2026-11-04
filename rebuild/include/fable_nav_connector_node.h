#pragma once
// Hand-owned (retail model): CNavConnectorNode, the navigation graph's link between two navigable quad-tree leaves.
// Ego_r.pdb: 8 bytes, Neighbour1 +0x00 / Neighbour2 +0x04 (CNavNavigableLeafNode*), non-virtual
// ??1CNavConnectorNode@@QAE@XZ (Ego_r 0x00BBE9C0, retail 0x00A780B0: unlinks itself from both leaves' Neighbours
// lists - not yet reconstructed). The nav graph owns connectors through CCountedPointer<CNavConnectorNode>.
#include "rebuild_abi.h"

class CNavNavigableLeafNode;

class CNavConnectorNode
{
public:
    ~CNavConnectorNode();

    CNavNavigableLeafNode* Neighbour1;   // +0x00
    CNavNavigableLeafNode* Neighbour2;   // +0x04
};

FABLE_STATIC_ASSERT(sizeof(CNavConnectorNode) == 0x08);
